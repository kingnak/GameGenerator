#include "ggmappinguicontroller.h"
#include <command/ggeditcommandfactory.h>
#include <command/ggcommandstack.h>
#include <command/ggmodelpagecommands.h>
#include <model/ggpage.h>

GGMappingUIController::GGMappingUIController(GGEditModel *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    m_fac = new GGEditCommandFactory(m_model);
    m_stack = new GGCommandStack;
}

GGMappingUIController::~GGMappingUIController()
{
    delete m_fac;
    delete m_stack;
}

GGCommandStack *GGMappingUIController::getCommandStack()
{
    return m_stack;
}

void GGMappingUIController::createLink(GGMappedContentPage *page, QRect rect)
{
    GGMappedLink lnk = GGMappedLink::rectangle(rect);
    m_stack->execute(m_fac->addMappedLink(page, lnk));
}

void GGMappingUIController::moveLink(GGMappedContentPage *page, int idx, QRect rect)
{
    GGMappedLink lnk = page->getLinkMap()[idx];
    lnk.setRectangle(rect);
    m_stack->execute(m_fac->setMappedLink(page, idx, lnk));
}

void GGMappingUIController::undo()
{
    m_stack->undo();
}

void GGMappingUIController::redo()
{
    m_stack->redo();
}

void GGMappingUIController::undoAll()
{
    while (m_stack->undoCommand()) {
        m_stack->undo();
    }
}

