hidden_function::CMD_2(playerid, const cmdtext[], &idx)
{
    new arg1[MAX_ARGUMENT_LENGTH];
    new targetid = CommandAnalyzer(playerid, cmdtext, idx, arg1, 2);
    if (targetid == playerid)
    {
        SendClientMessage(playerid, -1, !"Ќельз€ применить к самому себе.");
        return true;
    }
    if (!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, !"»грока нет в сети.");
        return true;
    }
    new arg2[MAX_ARGUMENT_LENGTH];
    new type = CommandAnalyzer(playerid, cmdtext, idx, arg2, 3);
    // проверка на соответствие типа
    if (type == 1)
    {
        new arg3[MAX_ARGUMENT_LENGTH];
        new denomination = CommandAnalyzer(playerid, cmdtext, idx, arg3, 4);
        // проверка на - достаточно ли номинала?
        if (_cash[playerid][cash__denomination_type_1] < denomination)
        {
            SendClientMessage(playerid, -1, !"” вас недостаточно номинала.");
            return true;
        }
        new arg4[MAX_ARGUMENT_LENGTH];
        new quantity = CommandAnalyzer(playerid, cmdtext, idx, arg4, 5);
        // проверка на - достаточно ли количества?
        if (_cash[playerid][cash__quantity_type_1] < quantity)
        {
            SendClientMessage(playerid, -1, !"” вас недостаточно количества.");
            return true;
        }
    }
    // проверка на соответствие типа
    else if (type == 2)
    {
        new arg3[MAX_ARGUMENT_LENGTH];
        new denomination = CommandAnalyzer(playerid, cmdtext, idx, arg3, 4);
        // проверка на - достаточно ли номинала?
        if (_cash[playerid][cash__denomination_type_2] < denomination)
        {
            SendClientMessage(playerid, -1, !"” вас недостаточно номинала.");
            return true;
        }
        new arg4[MAX_ARGUMENT_LENGTH];
        new quantity = CommandAnalyzer(playerid, cmdtext, idx, arg4, 5);
        // проверка на - достаточно ли количества?
        if (_cash[playerid][cash__quantity_type_2] < quantity)
        {
            SendClientMessage(playerid, -1, !"” вас недостаточно количества.");
            return true;
        }
    }
    else
    {
        SendClientMessage(playerid, -1, !"Ќеверный тип денег.");
        return true;
    }
    new sum = denomination * quantity; // сумма денег = номинал (ценность) * количество (ед./шт.)
    return true;
}