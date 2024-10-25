# Вторичный индекс в базе данных
Часто от хранилища данных требуется быстрый поиск по любому полю. Для этого конструируются вспомогательные структуры данных, ускоряющие поиск, они называются вторичными индексами (secondary index). Реализуйте такое хранилище для данных типа Record ниже:  
```objectivec
struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

class Database {
public:
  bool Put(const Record& record);
  const Record* GetById(const string& id) const;
  bool Erase(const string& id);

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const;

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const;

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const;
};

```
Требования:

 - Операция Put должна возвращать true, если вставка удалась, и false, если вставка не удалась, потому что в базе данных уже есть запись с таким же id. В последнем случае состояние базы данных не должно меняться.

 - Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id.

 - Операция Erase должна возвращать true, если удалось удалить элемент с заданным id, и false, если элемент с заданным id не был найден. В последнем случае состояние базы данных не должно меняться.

 - Операции RangeByTimestamp, RangeByKarma и AllByUser принимают функтор callback. Подразумевается, что он имеет оператор (), принимающий в качестве аргумента объект типа Record и возвращающий bool.

 - Операция RangeByTimestamp должна обходить все записи, у которых поле timestamp находится в диапазоне [low, high] (обе границы включены), и для каждой из них один раз вызывать callback. Если очередной вызов callback вернул false, итерация прекращается.

 - Операция RangeByKarma должна обходить все записи, у которых поле karma находится в диапазоне [low, high].

 - Операция AllByUser должна обходить все записи в хранилище данных, у которых поле user равно значению параметра user.

 - Так же, как и RangeByTimestamp, методы RangeByKarma и AllByUser должны вызывать callback для каждой записи, по которой они итерируются, и прекращать итерацию, если callback вернул false.

 - Параметр callback позволяет управлять процессом обхода: он должен возвращать true, если требуется продолжить обход найденных записей, и false в противном случае. Например, это позволит вывести первые 10 записей или найти первую запись, удовлетворяющую дополнительному критерию.

 - Время выполнения всех операций должно иметь в среднем сублинейную (например, константную или логарифмическую) зависимость от общего числа записей в базе данных.

 - Все границы интервалов - включительные, [low, high] — это наиболее универсальный интервальный запрос, подумайте, как с помощью него выразить условия вида x < high, x > low, x = x_0.

 - При решении этой задачи вы можете считать, что bad_alloc кидаться не будет.

### Примечание ###
В этой задаче может оказаться полезен контейнер multimap. Мы не рассматривали его в лекциях, однако вам не составит труда в нём разобраться — он отличается от обычного map только тем, что может хранить повторяющиеся ключи.  