#ifndef CARETAKER_H
#define CARETAKER_H

#include <deque>

template <typename Originator, typename Memento>
class Caretaker
{
public:
    Caretaker(Originator *);
    void createSnapshot();
    void undoOperation();
    void redoOperation();

    void eraseSnapshots();
private:
    Originator * const originator_ptr;
    std::deque<Memento> snapshots;

    static const size_t MAX_SNAPSHOTS_COUNT;
};

// ------------------------------------------------------------------------------------------------- //

template <typename Originator, typename Memento>
const size_t Caretaker<Originator, Memento>::MAX_SNAPSHOTS_COUNT = 20u;

template <typename Originator, typename Memento>
Caretaker<Originator, Memento>::Caretaker(Originator * _originator_ptr) : originator_ptr(_originator_ptr)
{}

template <typename Originator, typename Memento>
void Caretaker<Originator, Memento>::createSnapshot()
{
    if (snapshots.size() == MAX_SNAPSHOTS_COUNT) // remove oldest snapshot
        snapshots.pop_front();
    snapshots.emplace_back(originator_ptr->createMemento());
}

template <typename Originator, typename Memento>
void Caretaker<Originator, Memento>::undoOperation()
{
    if (snapshots.empty())
        return;
    originator_ptr->reinstateMemento(std::move(snapshots.back()));
    snapshots.pop_back();
}

template <typename Originator, typename Memento>
void Caretaker<Originator, Memento>::redoOperation()
{
    // need implementation
}

template <typename Originator, typename Memento>
void Caretaker<Originator, Memento>::eraseSnapshots()
{
    if (!snapshots.empty())
        snapshots = std::deque<Memento>();
}

#endif // CARETAKER_H