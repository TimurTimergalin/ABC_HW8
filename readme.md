# Домашняя работа 8

**Выполнил:** Тимергалин Тимур Маратович, БПИ227

## Решение

Задание выполнено на языке *C++*, код программы лежит в *main.cpp*, для запуска использовался *cmake*, конфигурация лежит в *CMakeLists.txt*.

Размеры векторов были взяты `50000000` и `100000000` вместо `50000000000` и `100000000000` соответственно, так как последние векторы весят 400 и 800 ГБ соответственно, и попытка их создания вызывает ошибку `bad_alloc`.

## Результаты

Результат выполнения программы записывается в *results.csv*, в той же папке, что и исполняемы файл (.exe)

## Анализ результата

Для анализа приведём результат запуска на нашей машине:

```
Число элементов в векторе,Число потоков,Время выполнения в миллисекундах
50000000,1,268
50000000,4,88
50000000,20,84
50000000,30,85
50000000,40,93
50000000,50,90
50000000,100,96
50000000,200,120
50000000,500,224
50000000,1000,435
100000000,1,520
100000000,4,202
100000000,20,166
100000000,30,168
100000000,40,168
100000000,50,169
100000000,100,186
100000000,200,217
100000000,500,303
100000000,1000,505
```

Из низ видно, что на данной машине создание более 20 отрицательно влияет на производительность программы.
