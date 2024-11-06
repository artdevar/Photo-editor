#ifndef COMMANDSHISTORY_H
#define COMMANDSHISTORY_H

#include <QObject>

class Command;

class CommandsHistory : public QObject
{
public:

  using CommandId = quint32;

  CommandsHistory(QObject * parent = nullptr);

  void push(Command * command);
  void clear();

  bool isUndoPossible() const;
  void undo();

  bool isRedoPossible() const;
  void redo();

  CommandId getLastExecutedCommandId() const;

protected:

  void popFront();
  void popBack();

protected:

  static constexpr qsizetype HISTORY_SIZE = 20;

  QList<QPair<Command*, CommandId>> m_Commands;
  qint32                            m_CurrentCommandIndex;
};

#endif // COMMANDSHISTORY_H
