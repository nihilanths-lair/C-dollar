enum Server
{
    _2_cash_denomination_type_1[6], // номинал (ценность) для монет
    _2_cash_denomination_type_2[7]  // номинал (ценность) для банкнот
}
new server[Server];
//
#define/**/MAX__CASH_DENOMINATION_TYPE_1/**/(6)
#define/**/MAX__CASH_DENOMINATION_TYPE_2/**/(7)
#define/**/MAX__CASH_QUANTITY_TYPE_1/*   **/(6)
#define/**/MAX__CASH_QUANTITY_TYPE_2/*    */(7)
#define/**/MAX__CASH_SUM_TYPE_1/*         */(6)
#define/**/MAX__CASH_SUM_TYPE_2/*         */(7)
enum Player
{
    _1name[10+1], bool:_1name_passed,
    //_1cash_coin[6],
    //_1cash_banknote[7],
    //_1cash[13],
    //_1cash_coin_x1, _1cash_coin_x5, _1cash_coin_x10, _1cash_coin_x25, _1cash_coin_x50, _1cash_coin_x100,
    //_1cash_banknote_x1, _1cash_banknote_x2, _1cash_banknote_x5, _1cash_banknote_x10, _1cash_banknote_x20, _1cash_banknote_x50, _1cash_banknote_x100,

    cash__denomination_type_1[MAX__CASH_DENOMINATION_TYPE_1], // номинал (ценность) монет
    cash__denomination_type_2[MAX__CASH_DENOMINATION_TYPE_2], // номинал (ценность) банкнот
    cash__quantity_type_1[MAX__CASH_QUANTITY_TYPE_1],          // количество монет
    cash__quantity_type_2[MAX__CASH_QUANTITY_TYPE_2],          // количество банкнот
    cash__sum_1[MAX__CASH_SUM_TYPE_1],                        // сумма денег (монеты)
    cash__sum_2[MAX__CASH_SUM_TYPE_2]                         // сумма денег (банкноты)
}
new players[MAX_PLAYERS][Player];
//
#define/**/NUMBER_OF_TEAMS/******/(3)
#define/**/NUMBER_OF_DIALOGUES/**/(3)
#define/**/NUMBER_OF_MESSAGES/***/(3)
//
#define/**/MAX_LENGTH_COMMAND_NAME/****/(20)
#define/**/MAX_LENGTH_DIALOG_INFO/***/(2048)
//
enum ServerCommand { server_command__id, server_command__name[MAX_LENGTH_COMMAND_NAME] } new server_command[NUMBER_OF_TEAMS][ServerCommand];
enum PlayerCommand { player_command__id, player_command__name[MAX_LENGTH_COMMAND_NAME] } new player_command[MAX_PLAYERS][NUMBER_OF_TEAMS][PlayerCommand];
//
enum ServerDialog { server_dialog__id, server_dialog__info[MAX_LENGTH_DIALOG_INFO] } new server_dialog[NUMBER_OF_DIALOGUES][ServerDialog];
enum PlayerDialog { player_dialog__id, player_dialog__info[MAX_LENGTH_DIALOG_INFO] } new player_dialog[MAX_PLAYERS][NUMBER_OF_DIALOGUES][PlayerDialog];
//
enum ServerMessage { server_message__id, server_message__color } new server_message[NUMBER_OF_MESSAGES][ServerMessage];
enum PlayerMessage { player_message__id, player_message__color } new player_message[MAX_PLAYERS][NUMBER_OF_MESSAGES][ServerMessage];
//
new MySQL:handle;
new File:fhandle;
//
new fstr[4095+1]; // секция данных/сегмент данных
//
new text_1[4095+1];
new text_2[4095+1];
new text_3[4095+1];