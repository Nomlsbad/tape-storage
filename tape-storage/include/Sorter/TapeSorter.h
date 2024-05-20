#ifndef TAPE_STORAGE_TAPE_SORTER_H
#define TAPE_STORAGE_TAPE_SORTER_H

#include "oneapi/tbb/concurrent_queue.h"
#include "Tape/Utils.h"
#include "Utils/Future.h"

#include <cassert>
#include <future>
#include <iostream>
#include <iterator>
#include <sstream>

namespace YTape
{

/**
 * TapeSorter implements asynchronous sorting for ITape's implementations.
 *
 * @tparam Comparator using for sorting in necessary order. std::less<> by default.
 *
 * @author Yurasov Ilia
 */
template<typename Comparator = std::less<>>
class TapeSorter
{
public:

    /**
     * TapeSorter's constructor.
     * Make sure that [begin, end) has at lest 4 elements and chunkLimit >= sizeof(int).
     *
     * @param begin first iterator in range.
     * @param end before the last iterator in range.
     * @param chunkLimit amount of bytes for inner chunk that takes parts of input and sorts it.
     * @param comparator callable object that takes 2 ints and returns bool
     *
     * @throws std::invalid_argument if chunkLimit less than sizeof(int)
     * or [begin, end) has 3 or less elements.
     *
     * @tparam Iter at least InputIterator. std::iterator_traits<Iter>::value_type would be casted to ITape*.
     */
     // TODO: Add check for tapes sizes
    template<typename Iter>
    explicit TapeSorter(Iter begin, Iter end, size_t chunkLimit, Comparator comparator)
        : chunkLimit_(chunkLimit / sizeof(int))
        , availableTapes_(std::distance(begin, end))
        , comparator_(std::move(comparator))
    {
        if (chunkLimit_ < 1)
        {
            throw std::invalid_argument("chunkLimit is too small.");
        }

        if (availableTapes_ < 4)
        {
            std::stringstream stringstream;
            stringstream << availableTapes_ << " tmp taps isn't enough. It should be 4 at least.";

            throw std::invalid_argument(stringstream.str());
        }

        ddTape_ = *begin;
        for (auto it = std::next(begin); it < end; ++it)
        {
            freeTapes_.push(*it);
        }
    }

    /**
     * TapeSorter's constructor.
     * Make sure that [begin, end) has at lest 4 elements and chunkLimit >= sizeof(int).
     * Comparator creates by default.
     *
     * @param begin first iterator in range.
     * @param end before the last iterator in range.
     * @param chunkLimit amount of bytes for inner chunk that takes parts of input and sorts it.
     *
     * @throws std::invalid_argument if chunkLimit less than sizeof(int)
     * or [begin, end) has 3 or less elements.
     *
     * @tparam Iter at least InputIterator. std::iterator_traits<Iter>::value_type would be casted to ITape*.
     */
    template<typename Iter>
    requires std::default_initializable<Comparator>
    explicit TapeSorter(Iter begin, Iter end, size_t chunkLimit)
        : TapeSorter(begin, end, chunkLimit, Comparator())
    {}

    TapeSorter(const TapeSorter&) = delete;
    TapeSorter& operator= (const TapeSorter&) = delete;

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

        ITape* tmp = nullptr;
        while (mergedTapes_.try_pop(tmp))
        {
            tmp->rewind();
            freeTapes_.push(tmp);
        }

        auto maxMerging = getMaxMerging(in.getSize(), chunkLimit_);
        auto mergingTask = std::async(&TapeSorter::doMerging, this, maxMerging);
        auto readingTask = std::async(std::launch::async, &TapeSorter::readInput, this, &in);

        readingTask.wait();
        mergingTask.wait();

        ITape* tape = nullptr;
        mergedTapes_.pop(tape);

        Utility::copy(*tape, out);
        mergedTapes_.push(tape);
    }

private:

    static size_t getMaxMerging(size_t tapeSize, size_t chunkSize) { return (tapeSize - 1) / chunkSize; }

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

        do {
            auto first = chunk.begin();
            auto last = chunk.end();

            loadChunk(in, first, last);
            std::sort(first, last, comparator_);

            ITape* tape = nullptr;
            freeTapes_.pop(tape);

            storeChunk(tape, first, last);
            mergedTapes_.push(tape);

        } while (in->next());
    }

    template<typename Iter>
    void loadChunk(ITape* in, const Iter& begin, Iter& end)
    {
        auto iter = begin;
        in->read(*iter);
        while (++iter != end && in->next())
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

private:

    void doMerging(size_t maxMerging)
    {
        std::vector<Task> tasks;
        MergeBlock block;

        while (mergingCounter_ < maxMerging)
        {
            releaseTasks(tasks);

            if (!popFree(block.out)) continue;
            if (!popMerged(block.leftIn)) continue;
            if (!popMerged(block.rightIn)) continue;

            auto mergeTask = std::async(&TapeSorter::merge, this, block);
            tasks.push_back(std::move(mergeTask));

            block = MergeBlock();
        }

        if (block.out != nullptr) freeTapes_.push(block.out);
        if (block.leftIn != nullptr) mergedTapes_.push(block.leftIn);
    }

    bool popFree(ITape*& tape)
    {
        if (tape == nullptr && !freeTapes_.try_pop(tape) && ddTape_ == nullptr) return false;
        if (tape == nullptr && ddTape_ != nullptr)
        {
            std::swap(tape, ddTape_);
        }
        return true;
    }

    bool popMerged(ITape*& tape) { return tape != nullptr || mergedTapes_.try_pop(tape); }

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

    void finishTask(std::future<MergeBlock>& task)
    {
        if (!task.valid()) return;

        auto [out, left, right] = task.get();

        assert(left->getPosition() == 0);
        assert(right->getPosition() == 0);
        assert(out->getPosition() != 0);

        mergedTapes_.push(out);
        freeTapes_.push(right);
        if (ddTape_ != nullptr)
        {
            freeTapes_.push(left);
        }
        else
        {
            ddTape_ = left;
        }

        ++mergingCounter_;
    }

    MergeBlock merge(MergeBlock block)
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
                comparator_(lValue, rValue) ? std::tie(lValue, lPosition, left) : std::tie(rValue, rPosition, right);

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

    using Queue = oneapi::tbb::concurrent_bounded_queue<ITape*>;

    Queue freeTapes_ {};
    Queue mergedTapes_ {};

    size_t chunkLimit_ {};
    std::ptrdiff_t availableTapes_ {};

    ITape* ddTape_ {};
    size_t mergingCounter_ {};

    Comparator comparator_;
};

} // namespace YTape

#endif // TAPE_STORAGE_TAPE_SORTER_H
