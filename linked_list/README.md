# Задание по программированию: Односвязный список
Односвязный список — классический пример структуры данных, для реализации которой нужно пользоваться памятью в куче. В этой задаче вам нужно реализовать шаблон класса LinkedList, который представляет собой односвязный список.
* Метод GetHead возвращает указатель на голову списка, он используется для перебора элементов списка (см. шаблон ToVector в заготовке решения)
* Метод PushFront добавляет новый элемент в голову списка.
* Метод InsertAfter вставляет новый элемент в список так, чтобы он шёл после узла node. Если node == nullptr, метод эквивалентен PushFront
* Метод PopFront удаляет элемент из головы списка и освобождает выделенную под него память. Если список пуст, метод завершается корректно. Если после выполнения метода список стал пустым, метод GetHead должен возвращать nullptr
* Метод RemoveAfter должен удалять из списка элемент, который следует за узлом node, и освобождать выделенную под него память. Если node == nullptr, метод эквивалентен PopFront. Если node->next == nullptr, то метод должен корректно завершаться.
* Все методы, перечисленные выше, должны работать за O(1)
* Деструктор класса LinkedList освобождает всю память, выделенную для хранения элементов списка.
