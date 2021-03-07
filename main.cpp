#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

string getMiddleString(const std::string &str)
{
    vector<string> temp;
    StringTools::split(str, ' ', temp);
    vector<string>::iterator it = temp.begin();
    it++;
    return *it;
}

string getEndString(const std::string &str)
{
    vector<string> temp;
    StringTools::split(str, ' ', temp);
    vector<string>::iterator it = temp.end();
    it--;
    return *it;
}

void forward_Message(Bot &bot, int64_t &admin_id)
{
    bot.getEvents().onCommand("r", [&bot, &admin_id](Message::Ptr message) {
        if(message->chat->id == admin_id)
        {
            string ID = getMiddleString(message->text);
            int64_t IDint = atoll(ID.c_str());
            string replyMessage = getEndString(message->text);
            bot.getApi().sendMessage(IDint, "[reply]: " + replyMessage);
            bot.getApi().sendMessage(message->chat->id, "Success!");
        }
        else
        {
            return;
        }
    });
    bot.getEvents().onAnyMessage([&bot, &admin_id](Message::Ptr message) {
        if(message->chat->id != admin_id)
        {
            if (StringTools::startsWith(message->text, "/start")) 
            {
                bot.getApi().sendMessage(message->chat->id, "請直接發送想要發送的消息。");
                return;
            }
            bot.getApi().forwardMessage(admin_id, message->chat->id, message->messageId);
            bot.getApi().sendMessage(message->chat->id, "Success!");
            string ID_md = std::string("<code>") + to_string(message->chat->id) + std::string("</code>");
            bot.getApi().sendMessage(admin_id, "用户ID：" + ID_md, false, 0, std::make_shared< GenericReply >(), "HTML");
        }
        else
        {
            return;
        }
    });
}

int main()
{
    string token("PLACE YOUR TOKEN HERE");      //在這裏輸入bot token
    int64_t admin_id = 0000000000;      //在這裏輸入想要連接的管理ID
    Bot bot(token);
    forward_Message(bot,admin_id);

    try
    {
        TgLongPoll longPoll(bot);
        while (true)
        {
            longPoll.start();
        }
    }
    catch (exception &e)
    {
        printf("error: %s\n", e.what());
    }

    return 0;
}