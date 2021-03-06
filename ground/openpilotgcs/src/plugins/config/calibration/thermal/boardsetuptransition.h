/**
 ******************************************************************************
 *
 * @file       boardsetuptransition.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2014.
 *
 * @brief
 * @see        The GNU Public License (GPL) Version 3
 * @defgroup
 * @{
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef BOARDSETUPTRANSITION_H
#define BOARDSETUPTRANSITION_H

#include "thermalcalibrationhelper.h"

#include <QSignalTransition>
#include <QEventTransition>

namespace OpenPilot {
class BoardSetupTransition : public QSignalTransition {
    Q_OBJECT

public:
    BoardSetupTransition(ThermalCalibrationHelper *helper, QState *currentState, QState *targetState)
        : QSignalTransition(helper, SIGNAL(setupBoardCompleted(bool))),
        m_helper(helper)
    {
        QObject::connect(currentState, SIGNAL(entered()), this, SLOT(enterState()));

        setTargetState(targetState);
    }

    virtual bool eventTest(QEvent *e)
    {
        if (!QSignalTransition::eventTest(e)) {
            return false;
        }
        QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent *>(e);

        // check wether status save was successful and retry if not
        if (se->arguments().at(0).toBool()) {
            return true;
        } else {
            m_helper->setupBoard();
        }
        return false;
    }

    virtual void onTransition(QEvent *e)
    {
        Q_UNUSED(e);
    }

public slots:
    void enterState()
    {
        m_helper->addInstructions(tr("Configuring board for calibration."), WizardModel::Debug);
        m_helper->setupBoard();
    }

private:
    ThermalCalibrationHelper *m_helper;
};
}

#endif // BOARDSETUPTRANSITION_H
