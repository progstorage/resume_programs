#include <iostream>
#include <ostream>
#include <fstream>
#include <cmath>
#include <set>


using namespace std;

using _pair = pair<int, int>;
using _set = set<_pair>;
using _iterator = pair<std::_Rb_tree<_pair,_pair,_Identity<_pair>,less<_pair>,
        __gnu_cxx::__alloc_traits<allocator<pair<int,int>>>::rebind<_pair>::other>::const_iterator,bool>;


ostream& operator<< (ostream& stream, _pair p) {
    /*Перегрузка оператора вывода для объекта типа _pair.*/		/*[функция для тестирования]*/
    stream << p.first << " " << p.second << endl;
    return stream;
}


void PrintSet(const _set& pairs) {
    /* Печатает множество пар. */	/*[функция для тестирования]*/
    for (const auto& p : pairs)
        cout << p;
}


inline double dist(const _pair& p1, const _pair& p2) {
    /* Возвращает расстояние между двумя точками на плоскости. */
    return sqrt(pow((p1.first - p2.first), 2) + pow((p1.second - p2.second), 2));
}


inline void count_dist_left(const _iterator it, double& min_d) {
    /* Рассчитывает расстояние до соседа слева.
     * И обновляет минимум, если новое расстояние меньше минимального.
     * */
    _iterator tmp = it;
    double d = dist(*it.first, *(--tmp.first));

    if (min_d > d)
        min_d = d;
}


inline void count_dist_right(const _iterator it, double& min_d) {
    /* Рассчитывает расстояние до соседа справа.
     * И обновляет минимум, если новое расстояние меньше минимального.
     * */
    _iterator tmp = it;
    double d = dist(*it.first, *(++tmp.first));

    if (min_d > d)
        min_d = d;
}


int main(void) {
    /* Дано: N пар {(x, y)}.
     * Найти: Наименьшее евклидово расстояние между двумя точками.
     * Алгоритм: Возьмем два множества (set), отсортированных по координатам x и y соответственно.
     * При каждой вставке проверяем расстояние до соседних точек (слева и справа).
     * Асимптотическая сложность: Вставка в множество - O(logN). Сравнение - O(1). Итоговая сложность
     * для одного мн-ва - O(N*(logN + 2)) = O(N*logN). Для двух множеств - O(2*N*logN) = O(N*logN).
     * */

    //string filename = "input.txt";
    //ifstream input(filename);

    unsigned int N;
    cin >> N;			// N - число пар

    _set pairs_x;		// множество пар точек, отсортированных по х координате
    _set pairs_y;		// -----------------------//----------- по y координате

    double x, y;		// переменные для чтения с входного потока

    double min_d_x = 2e+5, min_d_y = 2e+5;		// минимальные расстояния

    _iterator it_x, it_y;	// два итератора вставки

    unsigned int count = 0;
    while (count < N) {		// O(N*logN)
        cin >> x >> y;
        count++;

        it_x = pairs_x.insert({x, y});

        if (pairs_x.size() != count) {		// если новая пара совпала с уже имеющейся,
            cout << "0";					// то расстояние равно 0, и программа завершается
            return 0;
        }

        /* Не должны считать расстояние для первой пары.
         * Если текущая пара - крайняя слева, то считать расстояние только с правым соседом;
         * Если -----------//--------- справа, ----------------//----------- левым соседом.
         * */
        if (*it_x.first == *pairs_x.begin() && *it_x.first != *pairs_x.rbegin())
            count_dist_right(it_x, min_d_x);
        else if (*it_x.first == *pairs_x.rbegin() && *it_x.first != *pairs_x.begin())
            count_dist_left(it_x, min_d_x);
        else if (*it_x.first != *pairs_x.begin() && *it_x.first != *pairs_x.rbegin()){
            count_dist_left(it_x, min_d_x);
            count_dist_right(it_x, min_d_x);
        }

        it_y = pairs_y.insert({y, x});

        if (*it_y.first == *pairs_y.begin() && *it_y.first != *pairs_y.rbegin())
            count_dist_right(it_y, min_d_y);
        else if (*it_y.first == *pairs_y.rbegin() && *it_y.first != *pairs_y.begin())
            count_dist_left(it_y, min_d_y);
        else if (*it_y.first != *pairs_y.begin() && *it_y.first != *pairs_y.rbegin()) {
            count_dist_left(it_y, min_d_y);
            count_dist_right(it_y, min_d_y);
        }
    }

    cout << pow(min(min_d_x, min_d_y), 2);		// конечный ответ (квадрат минимального расстояния между точками)

    return 0;
}
