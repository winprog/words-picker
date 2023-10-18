/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) Ziqiang Xu
 *
 * Author:     Ziqiang Xu <ziqiang_xu@yeah.net>
 * Maintainer: Ziqiang Xu <ziqiang_xu@yeah.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "float_browser.h"
#include <QFocusEvent>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
// #include <QSound>
#include "../api/play_url.h"
#include "../defined.h"

FloatBrowser::FloatBrowser()
{
    setWindowTitle(tr("快速查词"));
    int width = 600;
    int height = 600;
    
    setFixedSize(width,height);
    setWindowFlags(Qt::Dialog);

    input = new QLineEdit(this);
    input->setGeometry(0,0,190,30);

    browser = new QTextBrowser(this);
    browser->setGeometry(0,30,width,height-60);
    INFO << "Float_browser object creating" << browser->size();

    btn_query = new QPushButton(this);
    btn_query->setGeometry(195, 0, 80, 30);
    btn_query->setText(tr("查询"));

//    add_new = new QPushButton(this);
//    add_new->setGeometry(5, 270, 80, 30);
//    add_new->setText(tr("加入生词"));
//    add_new->adjustSize();

    google_translate = new QPushButton(this);
    google_translate->setGeometry(280, 0, 180, 30);
    google_translate->setText(tr("Google翻译"));

    // add a button to read the query out.
    btn_voice = new QPushButton(this);
    btn_voice->setGeometry(465, 0, 80, 30);
    btn_voice->setText(tr("读"));

    signal_slot();
}

FloatBrowser::~FloatBrowser() {

}


void FloatBrowser::signal_slot()
{
    // todo 处理悬浮窗口的按钮点击
//    connect(btn_query, &QPushButton::clicked,
//            this, );
}

bool FloatBrowser::isMouseOn()
{
//    检查鼠标是否在窗口区域
    int cursor_x = QCursor::pos().x();
    int cursor_y = QCursor::pos().y();
    INFO << "pos_x:" << cursor_x << "pos_y:" << cursor_y;
    INFO << "x():" << x() << "y()" << y();
    INFO << "width:" <<width() << "height" << height();
    if (cursor_x > x() && cursor_x < (x() + width()) &&
            cursor_y > y() - 35 && cursor_y < (y() + height() + 35))
    {
        DEBUG << "Mouse on the browser";
        return true;
    }
    DEBUG << "Mouse out of the browser";
    return false;
}

void FloatBrowser::play_tts(QString *input)
{
#if 1
    INFO << "tts: " << input->toUtf8().data();
    QString url = tr("https://dict.youdao.com/dictvoice?audio=%1&type=2").arg(input->toUtf8().data());
    INFO << "audio url: " << url;
    play_audio_url(url.toUtf8().data());
#endif
    
    // QString url = "https://dict.youdao.com/dictvoice?audio=input&type=2";
    // QSound::play(url);
}

void FloatBrowser::google_web_translate(QString src_word,
                                         QString src_language,
                                         QString des_language)
{
//    用浏览器打开google网页翻译
    // Visite google translation by browser
    INFO << "src_language:" << src_language << " des_language:" << des_language;
    if (src_language == "zh_CHS") src_language = "zh-CN";
    if (src_language == "EN") src_language = "en";
    if (des_language == "zh_CHS") des_language = "zh-CN";
    if (des_language == "EN") des_language = "en";

    INFO << "源语言：" << src_language << "\n目标语言：" << des_language;
    // https://translate.google.com.hk/?hl=zh-CN&sourceid=cnhp&sl=auto&tl=zh-CN&text=time&op=translate
    // QUrl google_url(tr("https://translate.google.cn/#%1/%2/%3")
    QUrl google_url(tr("https://translate.google.com.hk/?hl=%1&sourceid=cnhp&sl=auto&tl=%2&text=%3&op=translate")
                    .arg(src_language)
                    .arg(des_language)
                    .arg(src_word));
    QDesktopServices::openUrl(google_url);
}

void FloatBrowser::closeEvent(QCloseEvent *event)
{
//    隐藏窗口而不是关闭窗口
    INFO << "Close clicked, hide the float window";
    this->hide();
    event->ignore();
}

void FloatBrowser::hideEvent(QHideEvent *event)
{
    // 按过最小化按钮之后就无法再次显示了
    INFO << "Minimize clicked, hide the float window";
    // this->close();
    // event->ignore();
}
