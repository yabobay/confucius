#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <concord/discord.h>
#include <concord/log.h>

#include "string.h"

const char *translate_text(const char*);

#define random(from, to) rand() % (to - from + 1) + from

char *getWisdom(bool translate) {
    srand(time(NULL));
    char *wisdom = strdup("");
    const short syllables[] = {5, 7, 5};
    const int n = sizeof(syllables) / sizeof(syllables[0]) - 1;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < syllables[i]; j++)
            catCodepoint(&wisdom, random(0x4E00, 0x9FFF));
        if (i != n)
            catChar(&wisdom, '\n');
    }
    if (translate) {
        char *result = translate_text(wisdom);
        cat(&wisdom, "\n\n");
        cat(&wisdom, result);
        free(result);
    }
    return wisdom;
}

void on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_global_application_command params = {
        .name = "wisdom",
        .description = "Generate some wisdom 🥠",
        .dm_permission = true
    };
    discord_create_global_application_command(client, event->application->id, &params, NULL);
}

void on_interaction(struct discord *client, const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return;
    if (!strcmp(event->data->name, "wisdom")) {
        char *wisdom = getWisdom(true);
        struct discord_interaction_response params = {
            .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
            .data = &(struct discord_interaction_callback_data) {.content=wisdom}
        };
        discord_create_interaction_response(client, event->id, event->token, &params, NULL);
        free(wisdom); // goodbye wisdom
    }
}

int main() {
    struct discord *client = discord_config_init("config.json");
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);
    discord_run(client);
}
