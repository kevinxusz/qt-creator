/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "ipcserverproxy.h"

#include <cmbalivemessage.h>
#include <cmbcompletecodemessage.h>
#include <cmbendmessage.h>
#include <cmbregisterprojectsforcodecompletionmessage.h>
#include <cmbregistertranslationunitsforcodecompletionmessage.h>
#include <cmbunregisterprojectsforcodecompletionmessage.h>
#include <cmbunregistertranslationunitsforcodecompletionmessage.h>
#include <ipcclientinterface.h>

#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>

namespace ClangBackEnd {

IpcServerProxy::IpcServerProxy(IpcClientInterface *client, QIODevice *ioDevice)
    : writeMessageBlock(ioDevice),
      readMessageBlock(ioDevice),
      client(client)
{
    QObject::connect(ioDevice, &QIODevice::readyRead, [this] () {IpcServerProxy::readMessages();});
}

void IpcServerProxy::readMessages()
{
    for (const QVariant &message : readMessageBlock.readAll())
        client->dispatch(message);
}

void IpcServerProxy::resetCounter()
{
    writeMessageBlock.resetCounter();
    readMessageBlock.resetCounter();
}

void IpcServerProxy::end()
{
    writeMessageBlock.write(QVariant::fromValue(EndMessage()));
}

void IpcServerProxy::registerTranslationUnitsForCodeCompletion(const RegisterTranslationUnitForCodeCompletionMessage &message)
{
    writeMessageBlock.write(QVariant::fromValue(message));
}

void IpcServerProxy::unregisterTranslationUnitsForCodeCompletion(const UnregisterTranslationUnitsForCodeCompletionMessage &message)
{
    writeMessageBlock.write(QVariant::fromValue(message));
}

void IpcServerProxy::registerProjectPartsForCodeCompletion(const RegisterProjectPartsForCodeCompletionMessage &message)
{
    writeMessageBlock.write(QVariant::fromValue(message));
}

void IpcServerProxy::unregisterProjectPartsForCodeCompletion(const UnregisterProjectPartsForCodeCompletionMessage &message)
{
    writeMessageBlock.write(QVariant::fromValue(message));
}

void IpcServerProxy::completeCode(const CompleteCodeMessage &message)
{
    writeMessageBlock.write(QVariant::fromValue(message));
}

} // namespace ClangBackEnd

