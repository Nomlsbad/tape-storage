# Yadro TATLIN Data Services

## Tape Sorete

После компиляции в дирректории bin будет лежать исполняемый файл tape-sorter.
Как и указано в условии на вход принимаются 2 параметра: путь к файлу входной и выходной лент.

```
./tape-sorter <input-tape-path> <output-tape-path>
```

Чтобы фалй мог использоваться, как лента, он должен иметь специальный формат.
Чтобы быстро создавать файлы в нужно формате я написал ещё одну утилитку tape-formatter.

## File Tape Formatter

В директории bin лежит утилита tape-formatter, которая может создать пустую ленту определённого размера.

```
Usage:   ./tape-formatter <tape-file-path> <size>
Exemple: ./tape-fotmatter "/home/ilia/empty-tape.txt" 10
```

Или ленту заполненную числами из другого файла. Каждое число в инициализирующем файле
должно быть записано с новой сторчи.

```
Usage:   ./tape-formatter <tape-file-path> -f <init-file-path>
Exemple: ./tape-fotmatter "/home/ilia/tape.txt" -f "home/ilia/tape-templates/tape_1.txt"
```

Есть ещё несколько варинатов использования

```
Usage:   ./tape-formatter <tape-file-path> -z <size>
Usage:   ./tape-formatter <tape-file-path> -r <size> <min> <max>
Usage:   ./tape-formatter <tape-file-path> -c <numbers...>
```
Первый генириует ленту размера size, заполненную 0. Второй заполняет ленту случайными числами из диапозона [min, max].
Третий заполняет ленту указанными числами.
