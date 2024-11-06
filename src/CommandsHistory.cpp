#include "CommandsHistory.h"
#include "Commands.h"

CommandsHistory::CommandsHistory(QObject * parent) :
  QObject(parent),
  m_Commands(),
  m_CurrentCommandIndex(-1)
{
  // Empty
}

void CommandsHistory::push(Command * command)
{
  static CommandId commandId = CommandId();

  command->setParent(this);

  while (m_CurrentCommandIndex != m_Commands.size() - 1)
    popBack();

  if (m_Commands.size() == HISTORY_SIZE)
    popFront();

  m_Commands.emplaceBack(command, ++commandId);
  m_CurrentCommandIndex = m_Commands.size() - 1;
}

void CommandsHistory::clear()
{
  while (!m_Commands.isEmpty())
    popBack();

  m_CurrentCommandIndex = -1;
}

bool CommandsHistory::isUndoPossible() const
{
  return m_CurrentCommandIndex >= 0 && m_CurrentCommandIndex < m_Commands.size();
}

void CommandsHistory::undo()
{
  Q_ASSERT(isUndoPossible());

  m_Commands[m_CurrentCommandIndex].first->cancel();
  m_CurrentCommandIndex--;
}

bool CommandsHistory::isRedoPossible() const
{
  return m_CurrentCommandIndex + 1 >= 0 && m_CurrentCommandIndex + 1 < m_Commands.size();
}

void CommandsHistory::redo()
{
  Q_ASSERT(isRedoPossible());

  m_Commands[m_CurrentCommandIndex + 1].first->execute();
  m_CurrentCommandIndex++;
}

CommandsHistory::CommandId CommandsHistory::getLastExecutedCommandId() const
{
  return m_Commands.isEmpty() || m_CurrentCommandIndex < 0 ?
           CommandsHistory::CommandId() :
           m_Commands[m_CurrentCommandIndex].second;
}

void CommandsHistory::popFront()
{
  QPair<Command*, CommandId> & command = m_Commands.front();

  command.first->setParent(nullptr);
  command.first->deleteLater();

  m_Commands.pop_front();
}

void CommandsHistory::popBack()
{
  QPair<Command*, CommandId> & command = m_Commands.back();

  command.first->setParent(nullptr);
  command.first->deleteLater();

  m_Commands.pop_back();
}
