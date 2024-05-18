#ifndef TAPE_STORAGE_TAPESORTER_H
#define TAPE_STORAGE_TAPESORTER_H

#include "oneapi/tbb/concurrent_queue.h"
#include "Tape/Utils.h"
#include "Utils/Future.h"

#include <cassert>
#include <future>
#include <iterator>
#include <sstream>

namespace YTape
{

/**
 * TapeSorter implements asynchronous sorting
 * for ITape's implementations.
 *
 * @author Yurasov Ilia
 */
class TapeSorter
{
public:

    /**
     * TapeSorter's constructor.
     * Make shue that [begin, end) has at lest 3 elements and chunkLimit >= sizeof(int)
     *
     * @tparam Iter at least InputIterator. std::iterator_traits<Iter>::value_type would be casted to ITape*.
     * @param begin first iterator in range.
     * @param end before the last iterator in range.
     * @param chunkLimit amount of bytes for inner chunk that takes parts of input and sorts it.
     */
    template<typename Iter>
    explicit TapeSorter(Iter begin, Iter end, size_t chunkLimit)
        : chunkLimit_(chunkLimit / sizeof(int))
        , awailableTapes_(std::distance(begin, end))
    {
        if (chunkLimit_ < 1)
        {
            throw std::invalid_argument("chunckLimit is too small.");
        }

        if (awailableTapes_ < 3)
        {
            std::stringstream stringstream;
            stringstream << awailableTapes_ << " tmp taps isn't enougt. It should be 3 at least.";

            throw std::invalid_argument(stringstream.str());
        }

        for (auto it = begin; it < end; ++it)
        {
            freeTapes_.push(*it);
        }
    }

public:

    /**
     * Sorts data from in tape and writes them to out tape.
     *
     * @param in tape with not sorted data.
     * @param out tape in which the sorted data will be written.
     */
    void sort(ITape& in, ITape& out)
    {
        in.rewind();
        out.rewind();

        auto hasReaded = std::async(std::launch::deferred, &TapeSorter::readInput, this, &in);
        auto hasMerged = std::async(std::launch::async, &TapeSorter::doMerging, this, std::move(hasReaded));

        hasMerged.wait();

        ITape* tape = nullptr;
        mergedTapes_.pop(tape);
        Utility::copy(*tape, out);
    }

private:

    struct MergeBlock
    {
        ITape* out = nullptr;
        ITape* leftIn = nullptr;
        ITape* rightIn = nullptr;
    };

    using Task = std::future<MergeBlock>;

private:

    void readInput(ITape* in)
    {
        std::vector<int> chunk(chunkLimit_);
        assert(chunk.begin() != chunk.end());

        Task helpingTask;

        do {
            auto first = chunk.begin();
            auto last = chunk.end();

            loadChunk(in, first, last);
            std::sort(first, last);

            finishTask(helpingTask);

            ITape* tape = nullptr;
            freeTapes_.pop(tape);
            storeChunk(tape, first, last);

            // If it's true then other thread is blocked and is waiting for an element from the freeTape_.
            // We own one tape and all other tapes are placed in the mergedTapes_. It means
            // the size() call is safe and under this condition we need help with merging.
            if (mergedTapes_.size() == awailableTapes_ - 1)
            {
                runHelpingTask(helpingTask, tape);
            }
            else
            {
                mergedTapes_.push(tape);
            }

        } while (in->next());
    }

    template<typename Iter>
    void loadChunk(ITape* in, const Iter& begin, Iter& end)
    {
        auto iter = begin;
        in->read(*iter);
        while (++iter != end || in->next())
        {
            in->read(*iter);
        }
        end = iter;
    }

    template<typename Iter>
    void storeChunk(ITape* out, const Iter& begin, const Iter& end)
    {
        for (auto it = begin; it < end; ++it)
        {
            out->write(*it);
            out->next();
        }
    }

    void runHelpingTask(Task& task, ITape* out)
    {
        MergeBlock block;

        block.out = out;
        mergedTapes_.pop(block.leftIn);
        mergedTapes_.pop(block.rightIn);

        task = std::async(merge, block);
    }

private:

    void doMerging(std::future<void> inputReaded)
    {
        std::vector<Task> tasks;
        while (!isReady(inputReaded) || !tasks.empty())
        {
            MergeBlock block;
            freeTapes_.pop(block.out);
            mergedTapes_.pop(block.leftIn);
            mergedTapes_.pop(block.rightIn);

            auto mergeTask = std::async(merge, block);
            tasks.push_back(std::move(mergeTask));

            releaseTasks(tasks);
        }

        inputReaded.wait();
    }

    void releaseTasks(std::vector<Task>& tasks)
    {
        for (size_t i = 0; i < tasks.size(); ++i)
        {
            if (!isReadyOrDeffered(tasks[i])) continue;

            finishTask(tasks[i]);

            std::swap(tasks[i], tasks.back());
            tasks.pop_back();
        }
    }

    static MergeBlock merge(MergeBlock block)
    {
        auto [out, left, right] = block;

        const size_t lSize = left->getPosition();
        const size_t rSize = right->getPosition();

        left->rewind();
        right->rewind();

        size_t lPosition = 0;
        size_t rPosition = 0;

        while (lPosition < lSize && rPosition < rSize)
        {
            int lValue = 0;
            left->read(lValue);

            int rValue = 0;
            right->read(rValue);

            auto [value, position, tape] =
                lValue < rValue ? std::tie(lValue, lPosition, left) : std::tie(rValue, rPosition, right);

            out->write(value);
            out->next();
            tape->next();
            ++position;
        }

        if (lPosition != lSize || rPosition != rSize)
        {
            auto [position, size, tape] =
                lPosition < lSize ? std::tie(lPosition, lSize, left) : std::tie(rPosition, rSize, right);
            while (position < size)
            {
                int value = 0;
                tape->read(value);
                tape->next();

                out->write(value);
                out->next();

                ++position;
            }
        }

        left->rewind();
        right->rewind();

        return block;
    }

private:

    void finishTask(std::future<MergeBlock>& task)
    {
        if (!task.valid()) return;

        auto [out, left, right] = task.get();

        assert(left->getPosition() == 0);
        assert(right->getPosition() == 0);
        assert(out->getPosition() != 0);

        freeTapes_.push(left);
        freeTapes_.push(right);
        mergedTapes_.push(out);
    }

private:

    using Queue = oneapi::tbb::concurrent_bounded_queue<ITape*>;

    size_t chunkLimit_;
    std::ptrdiff_t awailableTapes_;

    Queue freeTapes_;
    Queue mergedTapes_;
};

} // namespace YTape

#endif // TAPE_STORAGE_TAPESORTER_H
