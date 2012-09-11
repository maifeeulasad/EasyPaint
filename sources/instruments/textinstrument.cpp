/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "textinstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"
#include "../undocommand.h"
#include "../dialogs/textdialog.h"

#include <QPainter>

TextInstrument::TextInstrument(QObject *parent) :
    AbstractSelection(parent)
{
    mText = QString();
    mIsEdited = false;
}

void TextInstrument::updateText(ImageArea *imageArea, QString textString)
{
    mText = textString;
    imageArea->setImage(mImageCopy);
    if (!mIsEdited)
    {
        makeUndoCommand(*imageArea);
        mIsEdited = true;
    }
    paint(*imageArea);
    drawBorder(*imageArea);
}

void TextInstrument::startSelection(ImageArea &imageArea)
{
}

void TextInstrument::startResizing(ImageArea &imageArea)
{
}

void TextInstrument::startMoving(ImageArea &imageArea)
{
}

void TextInstrument::select(ImageArea &imageArea)
{
}

void TextInstrument::resize(ImageArea &imageArea)
{
    paint(imageArea);
}

void TextInstrument::move(ImageArea &imageArea)
{
    paint(imageArea);
}

void TextInstrument::completeSelection(ImageArea &imageArea)
{
    TextDialog *td = new TextDialog(&imageArea);
    connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
    connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(close()));
    td->setAttribute(Qt::WA_DeleteOnClose);
    td->show();
}

void TextInstrument::completeResizing(ImageArea &imageArea)
{
}

void TextInstrument::completeMoving(ImageArea &imageArea)
{
}

void TextInstrument::clear(ImageArea &imageArea)
{
    mText = QString();
    mIsEdited = false;
    emit sendCloseTextDialog();
}

void TextInstrument::paint(ImageArea &imageArea, bool isSecondaryColor, bool additionalFlag)
{
    if(mTopLeftPoint != mBottomRightPoint)
    {
        QPainter painter(imageArea.getImage());
        painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
        painter.setFont(DataSingleton::Instance()->getTextFont());
        painter.drawText(QRect(mTopLeftPoint, mBottomRightPoint), mText);
        painter.end();
        imageArea.setEdited(true);
        imageArea.update();
    }
}
