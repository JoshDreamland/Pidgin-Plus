extern const int NUM_MSGPLUS_COMMANDS;

extern PurpleCmdId COMMANDS_MSGPLUS[];
extern const char *command_string_msgplus[];
extern const char *command_description_msgplus[];

bool execute_command(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *error, void *data);
