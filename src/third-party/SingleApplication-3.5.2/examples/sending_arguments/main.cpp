// Copyright (c) Itay Grudev 2015 - 2023
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// Permission is not granted to use this software or any of the associated files
// as sample data for the purposes of building machine learning models.
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <singleapplication.h>

#include "messagereceiver.h"

int main(int argc, char *argv[]) {
  // Allow secondary instances
  SingleApplication app(argc, argv, true);

  MessageReceiver msgReceiver;

  // If this is a secondary instance
  if (app.isSecondary()) {
    app.sendMessage(app.arguments().join(' ').toUtf8());
    qDebug() << "App already running.";
    qDebug() << "Primary instance PID: " << app.primaryPid();
    qDebug() << "Primary instance user: " << app.primaryUser();
    return 0;
  } else {
    QObject::connect(&app, &SingleApplication::receivedMessage, &msgReceiver,
                     &MessageReceiver::receivedMessage);
  }

  return app.exec();
}
