#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>

#include <pthread.h>

// Контейнер для хранения результата скалярного произведения
// со встроенным мьютексом - для потокобезопасности
class Result {
private:
    pthread_mutex_t mtx;
    double res;
public:
    Result() : mtx(), res(0) {
        pthread_mutex_init(&mtx, nullptr);
    }

    void Add(double n) {
        pthread_mutex_lock(&mtx);
        res += n;
        pthread_mutex_unlock(&mtx);
    }

    double Get() {
        pthread_mutex_lock(&mtx);
        double r = res;
        pthread_mutex_unlock(&mtx);
        return r;
    }

    ~Result() {
        pthread_mutex_destroy(&mtx);
    }
};

// Класс, отвечающий за выполнение задания
// Все поля - аргументы, необходимые для задачи
struct Task {
    const std::vector<double> &vec1;
    const std::vector<double> &vec2;
    const size_t start;
    const size_t end;
    Result &res;

    // Функция, которая будет передаваться в pthread_create
    static void *Handle(void *this_) {
        Task &cst = *reinterpret_cast<Task *>(this_);
        Routine(cst.vec1, cst.vec2, cst.start, cst.end, cst.res);
        return nullptr;
    }

    // Непосредственно логика программы потока
    static void
    Routine(const std::vector<double> &vec1, const std::vector<double> &vec2, size_t start, size_t end, Result &res) {
        double sum = 0;

        for (size_t i = start; i < end; ++i) {
            sum += vec1[i] * vec2[i];
        }

        res.Add(sum);
    }
};

// Непосредственно подсчёт скалярного произведения
int64_t experiment(size_t vec_size, size_t threads_count) {
    std::vector<pthread_t> threads(threads_count);
    std::vector<double> a(vec_size);
    std::vector<double> b(vec_size);

    for (size_t i = 0; i < vec_size; ++i) {
        a[i] = static_cast<double>(i + 1);
        b[i] = static_cast<double>(vec_size - i);
    }

    Result res{};

    auto start = std::chrono::high_resolution_clock::now();
    size_t d = vec_size / threads_count;
    for (size_t i = 0; i < threads_count; ++i) {
        Task t{a, b, i * d, std::min(i * d + d, vec_size), res};
        pthread_create(&threads[i], nullptr, Task::Handle, reinterpret_cast<void *>(&t));
    }
    for (size_t i = 0; i < threads_count; ++i) {
        pthread_join(threads[i], nullptr);
    }
    auto dur = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}


int main() {
    // Проведение экспериментов на разных входных данных и запись результатов в файл
    // Запись в файл синхронная, синхронизация не требуется
    std::ofstream file;
    file.open("results.csv");
    file << "Число элементов в векторе,Число потоков,Время выполнения в миллисекундах\n";

    size_t sizes[] {50000000, 100000000};
    size_t threads_counts[] {1, 4, 20, 30, 40, 50, 100, 200, 500, 1000};

    for (auto x : sizes) {
        for (auto y: threads_counts) {
            file << x << ',' << y << ',' << experiment(x, y) << '\n';
        }
    }

    file.close();

    return 0;
}
