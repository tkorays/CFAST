#include "cfx/ascii_art.h"
#include "cf/memory.h"
#include "cf/str.h"


/** map start from */
#define CFX_ART_MAP_BEGIN 33
#define CFX_ART_MAP_END 127
#define CFX_ART_MAX_LINES 6
static const cf_char_t* ascii_map[94] = {
    /** ! " # $ % & ' ( ) * + ， - . / */
    " _ \n| |\n| |\n|_|\n(_)\n   \n",
    " _ _ \n( | )\n V V \n     \n     \n     \n",
    "   _  _   \n _| || |_ \n|_  ..  _|\n|_      _|\n  |_||_|  \n          \n",
    "  _  \n | | \n/ __)\n\\__ \\\n(   /\n |_| \n",
    " _  __\n(_)/ /\n  / / \n / /_ \n/_/(_)\n      \n",
    "  ___   \n ( _ )  \n / _ \\/\\\n| (_>  <\n \\___/\\/\n        \n",
    " _ \n( )\n|/ \n   \n   \n   \n",
    "  __\n / /\n| | \n| | \n| | \n \\_\\\n",
    "__  \n\\ \\ \n | |\n | |\n | |\n/_/ \n",
    "      \n__/\\__\n\\    /\n/_  _\\\n  \\/  \n      \n",
    "       \n   _   \n _| |_ \n|_   _|\n  |_|  \n       \n",
    "   \n   \n   \n _ \n( )\n|/ \n",
    "       \n       \n _____ \n|_____|\n       \n       \n",
    "   \n   \n   \n _ \n(_)\n   \n",
    "    __\n   / /\n  / / \n / /  \n/_/   \n      \n",

    /** 0 1 2 3 4 5 6 7 8 9 : ; < = > ? */
    "  ___  \n / _ \\ \n| | | |\n| |_| |\n \\___/ \n       \n",
    " _ \n/ |\n| |\n| |\n|_|\n   \n",
    " ____  \n|___ \\ \n  __) |\n / __/ \n|_____|\n       \n",
    " _____ \n|___ / \n  |_ \\ \n ___) |\n|____/ \n       \n",
    " _  _   \n| || |  \n| || |_ \n|__   _|\n   |_|  \n        \n",
    " ____  \n| ___| \n|___ \\ \n ___) |\n|____/ \n       \n",
    "  __   \n / /_  \n| '_ \\ \n| (_) |\n \\___/ \n       \n",
    " _____ \n|___  |\n   / / \n  / /  \n /_/   \n       \n",
    "  ___  \n ( _ ) \n / _ \\ \n| (_) |\n \\___/ \n       \n",
    "  ___  \n / _ \\ \n| (_) |\n \\__, |\n   /_/ \n       \n",
    "   \n _ \n(_)\n _ \n(_)\n   \n",
    "   \n _ \n(_)\n _ \n( )\n|/ \n",
    "  __\n / /\n/ / \n\\ \\ \n \\_\\\n    \n",
    "       \n _____ \n|_____|\n|_____|\n       \n       \n",
    "__  \n\\ \\ \n \\ \\\n / /\n/_/ \n    \n",
    " ___ \n|__ \\\n  / /\n |_| \n (_) \n     \n",

    /** @ A B C D E F G H I J K L M N O */
    "   ____  \n  / __ \\ \n / / _` |\n| | (_| |\n \\ \\__,_|\n  \\____/ \n",
    "    _    \n   / \\   \n  / _ \\  \n / ___ \\ \n/_/   \\_\\\n         \n",
    " ____  \n| __ ) \n|  _ \\ \n| |_) |\n|____/ \n       \n",
    "  ____ \n / ___|\n| |    \n| |___ \n \\____|\n       \n",
    " ____  \n|  _ \\ \n| | | |\n| |_| |\n|____/ \n       \n",
    " _____ \n| ____|\n|  _|  \n| |___ \n|_____|\n       \n",
    " _____ \n|  ___|\n| |_   \n|  _|  \n|_|    \n       \n",
    "  ____ \n / ___|\n| |  _ \n| |_| |\n \\____|\n       \n",
    " _   _ \n| | | |\n| |_| |\n|  _  |\n|_| |_|\n       \n",
    " ___ \n|_ _|\n | | \n | | \n|___|\n     \n",
    "     _ \n    | |\n _  | |\n| |_| |\n \\___/ \n       \n",
    " _  __\n| |/ /\n| ' / \n| . \\ \n|_|\\_\\\n      \n",
    " _     \n| |    \n| |    \n| |___ \n|_____|\n       \n",
    " __  __ \n|  \\/  |\n| |\\/| |\n| |  | |\n|_|  |_|\n        \n",
    " _   _ \n| \\ | |\n|  \\| |\n| |\\  |\n|_| \\_|\n       \n",
    "  ___  \n / _ \\ \n| | | |\n| |_| |\n \\___/ \n       \n",

    /** P Q R S T U V W X Y Z [ \ ] ^ _ */
    " ____  \n|  _ \\ \n| |_) |\n|  __/ \n|_|    \n       \n",
    "  ___  \n / _ \\ \n| | | |\n| |_| |\n \\__\\_\\\n       \n",
    " ____  \n|  _ \\ \n| |_) |\n|  _ < \n|_| \\_\\\n       \n",
    " ____  \n/ ___| \n\\___ \\ \n ___) |\n|____/ \n       \n",
    " _____ \n|_   _|\n  | |  \n  | |  \n  |_|  \n       \n",
    " _   _ \n| | | |\n| | | |\n| |_| |\n \\___/ \n       \n",
    "__     __\n\\ \\   / /\n \\ \\ / / \n  \\ V /  \n   \\_/   \n         \n",
    "__        __\n\\ \\      / /\n \\ \\ /\\ / / \n  \\ V  V /  \n   \\_/\\_/   \n            \n",
    "__  __\n\\ \\/ /\n \\  / \n /  \\ \n/_/\\_\\\n      \n",
    "__   __\n\\ \\ / /\n \\ V / \n  | |  \n  |_|  \n       \n",
    " _____\n|__  /\n  / / \n / /_ \n/____|\n      \n",
    " __ \n| _|\n| | \n| | \n| | \n|__|\n",
    "__    \n\\ \\   \n \\ \\  \n  \\ \\ \n   \\_\\\n      \n",
    " __ \n|_ |\n | |\n | |\n | |\n|__|\n",
    " /\\ \n|/\\|\n    \n    \n    \n    \n",
    "       \n       \n       \n       \n _____ \n|_____|\n",

    /** ` a b c d e f g h i j k l m n o */
    " _ \n( )\n \\|\n   \n   \n   \n",
    "       \n  __ _ \n / _` |\n| (_| |\n \\__,_|\n       \n",
    " _     \n| |__  \n| '_ \\ \n| |_) |\n|_.__/ \n       \n",
    "      \n  ___ \n / __|\n| (__ \n \\___|\n      \n",
    "     _ \n  __| |\n / _` |\n| (_| |\n \\__,_|\n       \n",
    "      \n  ___ \n / _ \\\n|  __/\n \\___|\n      \n",
    "  __ \n / _|\n| |_ \n|  _|\n|_|  \n     \n",
    "       \n  __ _ \n / _` |\n| (_| |\n \\__, |\n |___/ \n",
    " _     \n| |__  \n| '_ \\ \n| | | |\n|_| |_|\n       \n",
    " _ \n(_)\n| |\n| |\n|_|\n   \n",
    "   _ \n  (_)\n  | |\n  | |\n _/ |\n|__/ \n",
    " _    \n| | __\n| |/ /\n|   < \n|_|\\_\\\n      \n",
    " _ \n| |\n| |\n| |\n|_|\n   \n",
    "           \n _ __ ___  \n| '_ ` _ \\ \n| | | | | |\n|_| |_| |_|\n           \n",
    "       \n _ __  \n| '_ \\ \n| | | |\n|_| |_|\n       \n",
    "       \n  ___  \n / _ \\ \n| (_) |\n \\___/ \n       \n",

    /** p q r s t u v w x y z { | } ~ */
    "       \n _ __  \n| '_ \\ \n| |_) |\n| .__/ \n|_|    \n",
    "       \n  __ _ \n / _` |\n| (_| |\n \\__, |\n    |_|\n",
    "      \n _ __ \n| '__|\n| |   \n|_|   \n      \n",
    "     \n ___ \n/ __|\n\\__ \\\n|___/\n     \n",
    " _   \n| |_ \n| __|\n| |_ \n \\__|\n     \n",
    "       \n _   _ \n| | | |\n| |_| |\n \\__,_|\n       \n",
    "       \n__   __\n\\ \\ / /\n \\ V / \n  \\_/  \n       \n",
    "          \n__      __\n\\ \\ /\\ / /\n \\ V  V / \n  \\_/\\_/  \n          \n",
    "      \n__  __\n\\ \\/ /\n >  < \n/_/\\_\\\n      \n",
    "       \n _   _ \n| | | |\n| |_| |\n \\__, |\n |___/ \n",
    "     \n ____\n|_  /\n / / \n/___|\n     \n",
    "   __\n  / /\n | | \n< <  \n | | \n  \\_\\\n",
    " _ \n| |\n| |\n| |\n| |\n|_|\n",
    "__   \n\\ \\  \n | | \n  > >\n | | \n/_/  \n",
    " /\\/|\n|/\\/ \n     \n     \n     \n     \n'"
};

static const cf_char_t* ascii_map_slant[94] = {
    "    __\n   / /\n  / / \n /_/  \n(_)   \n      \n",
    " _ _ \n( | )\n|/|/ \n     \n     \n     \n",
    "     __ __ \n  __/ // /_\n /_  _  __/\n/_  _  __/ \n /_//_/    \n           \n",
    "     __\n   _/ /\n  / __/\n (_  ) \n/  _/  \n/_/    \n",
    "   _   __\n  (_)_/_/\n   _/_/  \n _/_/_   \n/_/ (_)  \n         \n",
    "   ___   \n  ( _ )  \n / __ \\/|\n/ /_/  < \n\\____/\\/ \n         \n",
    "  _ \n ( )\n |/ \n    \n    \n    \n",
    "     __\n   _/_/\n  / /  \n / /   \n/ /    \n|_|    \n",
    "     _ \n    | |\n    / /\n   / / \n _/_/  \n/_/    \n",
    "       \n  __/|_\n |    /\n/_ __| \n |/    \n       \n",
    "       \n    __ \n __/ /_\n/_  __/\n /_/   \n       \n",
    "   \n   \n   \n _ \n( )\n|/ \n",
    "       \n       \n ______\n/_____/\n       \n       \n",
    "   \n   \n   \n _ \n(_)\n   \n",
    "       __\n     _/_/\n   _/_/  \n _/_/    \n/_/      \n         \n",
    "   ____ \n  / __ \\\n / / / /\n/ /_/ / \n\\____/  \n        \n",
    "   ___\n  <  /\n  / / \n / /  \n/_/   \n      \n",
    "   ___ \n  |__ \\\n  __/ /\n / __/ \n/____/ \n       \n",
    "   _____\n  |__  /\n   /_ < \n ___/ / \n/____/  \n        \n",
    "   __ __\n  / // /\n / // /_\n/__  __/\n  /_/   \n        \n",
    "    ______\n   / ____/\n  /___ \\  \n ____/ /  \n/_____/   \n          \n",
    "   _____\n  / ___/\n / __ \\ \n/ /_/ / \n\\____/  \n        \n",
    " _____\n/__  /\n  / / \n / /  \n/_/   \n      \n",
    "   ____ \n  ( __ )\n / __  |\n/ /_/ / \n\\____/  \n        \n",
    "   ____ \n  / __ \\\n / /_/ /\n \\__, / \n/____/  \n        \n",
    "     \n   _ \n  (_)\n _   \n(_)  \n     \n",
    "     \n   _ \n  (_)\n _   \n( )  \n|/   \n",
    "  __\n / /\n/ / \n\\ \\ \n \\_\\\n    \n",
    "       \n  _____\n /____/\n/____/ \n       \n       \n",
    "__  \n\\ \\ \n \\ \\\n / /\n/_/ \n    \n",
    "  ___ \n /__ \\\n  / _/\n /_/  \n(_)   \n      \n",
    "   ______ \n  / ____ \\\n / / __ `/\n/ / /_/ / \n\\ \\__,_/  \n \\____/   \n",
    "    ___ \n   /   |\n  / /| |\n / ___ |\n/_/  |_|\n        \n",
    "    ____ \n   / __ )\n  / __  |\n / /_/ / \n/_____/  \n         \n",
    "   ______\n  / ____/\n / /     \n/ /___   \n\\____/   \n         \n",
    "    ____ \n   / __ \\\n  / / / /\n / /_/ / \n/_____/  \n         \n",
    "    ______\n   / ____/\n  / __/   \n / /___   \n/_____/   \n          \n",
    "    ______\n   / ____/\n  / /_    \n / __/    \n/_/       \n          \n",
    "   ______\n  / ____/\n / / __  \n/ /_/ /  \n\\____/   \n         \n",
    "    __  __\n   / / / /\n  / /_/ / \n / __  /  \n/_/ /_/   \n          \n",
    "    ____\n   /  _/\n   / /  \n _/ /   \n/___/   \n        \n",
    "       __\n      / /\n __  / / \n/ /_/ /  \n\\____/   \n         \n",
    "    __ __\n   / //_/\n  / ,<   \n / /| |  \n/_/ |_|  \n         \n",
    "    __ \n   / / \n  / /  \n / /___\n/_____/\n       \n",
    "    __  ___\n   /  |/  /\n  / /|_/ / \n / /  / /  \n/_/  /_/   \n           \n",
    "    _   __\n   / | / /\n  /  |/ / \n / /|  /  \n/_/ |_/   \n          \n",
    "   ____ \n  / __ \\\n / / / /\n/ /_/ / \n\\____/  \n        \n",
    "    ____ \n   / __ \\\n  / /_/ /\n / ____/ \n/_/      \n         \n",
    "   ____ \n  / __ \\\n / / / /\n/ /_/ / \n\\___\\_\\ \n        \n",
    "    ____ \n   / __ \\\n  / /_/ /\n / _, _/ \n/_/ |_|  \n         \n",
    "   _____\n  / ___/\n  \\__ \\ \n ___/ / \n/____/  \n        \n",
    "  ______\n /_  __/\n  / /   \n / /    \n/_/     \n        \n",
    "   __  __\n  / / / /\n / / / / \n/ /_/ /  \n\\____/   \n         \n",
    " _    __\n| |  / /\n| | / / \n| |/ /  \n|___/   \n        \n",
    " _       __\n| |     / /\n| | /| / / \n| |/ |/ /  \n|__/|__/   \n           \n",
    "   _  __\n  | |/ /\n  |   / \n /   |  \n/_/|_|  \n        \n",
    "__  __\n\\ \\/ /\n \\  / \n / /  \n/_/   \n      \n",
    " _____\n/__  /\n  / / \n / /__\n/____/\n      \n",
    "     ___\n    / _/\n   / /  \n  / /   \n / /    \n/__/    \n",
    "__    \n\\ \\   \n \\ \\  \n  \\ \\ \n   \\_\\\n      \n",
    "     ___\n    /  /\n    / / \n   / /  \n _/ /   \n/__/    \n",
    "  //|\n |/||\n     \n     \n     \n     \n",
    "       \n       \n       \n       \n ______\n/_____/\n",
    "  _ \n ( )\n  V \n    \n    \n    \n",
    "        \n  ____ _\n / __ `/\n/ /_/ / \n\\__,_/  \n        \n",
    "    __  \n   / /_ \n  / __ \\\n / /_/ /\n/_.___/ \n        \n",
    "       \n  _____\n / ___/\n/ /__  \n\\___/  \n       \n",
    "       __\n  ____/ /\n / __  / \n/ /_/ /  \n\\__,_/   \n         \n",
    "      \n  ___ \n / _ \\\n/  __/\n\\___/ \n      \n",
    "    ____\n   / __/\n  / /_  \n / __/  \n/_/     \n        \n",
    "         \n   ____ _\n  / __ `/\n / /_/ / \n \\__, /  \n/____/   \n",
    "    __  \n   / /_ \n  / __ \\\n / / / /\n/_/ /_/ \n        \n",
    "    _ \n   (_)\n  / / \n / /  \n/_/   \n      \n",
    "       _ \n      (_)\n     / / \n    / /  \n __/ /   \n/___/    \n",
    "    __  \n   / /__\n  / //_/\n / ,<   \n/_/|_|  \n        \n",
    "    __\n   / /\n  / / \n / /  \n/_/   \n      \n",
    "            \n   ____ ___ \n  / __ `__ \\\n / / / / / /\n/_/ /_/ /_/ \n            \n",
    "        \n   ____ \n  / __ \\\n / / / /\n/_/ /_/ \n        \n",
    "       \n  ____ \n / __ \\\n/ /_/ /\n\\____/ \n       \n",
    "         \n    ____ \n   / __ \\\n  / /_/ /\n / .___/ \n/_/      \n",
    "        \n  ____ _\n / __ `/\n/ /_/ / \n\\__, /  \n  /_/   \n",
    "        \n   _____\n  / ___/\n / /    \n/_/     \n        \n",
    "        \n   _____\n  / ___/\n (__  ) \n/____/  \n        \n",
    "   __ \n  / /_\n / __/\n/ /_  \n\\__/  \n      \n",
    "        \n  __  __\n / / / /\n/ /_/ / \n\\__,_/  \n        \n",
    "       \n _   __\n| | / /\n| |/ / \n|___/  \n       \n",
    "          \n _      __\n| | /| / /\n| |/ |/ / \n|__/|__/  \n          \n",
    "        \n   _  __\n  | |/_/\n _>  <  \n/_/|_|  \n        \n",
    "         \n   __  __\n  / / / /\n / /_/ / \n \\__, /  \n/____/   \n",
    "     \n ____\n/_  /\n / /_\n/___/\n     \n",
    "     __\n   _/_/\n _/_/  \n< <    \n/ /    \n\\_\\    \n",
    "     __\n    / /\n   / / \n  / /  \n / /   \n/_/    \n",
    "     _ \n    | |\n    / /\n   _>_>\n _/_/  \n/_/    \n",
    "  /\\//\n //\\/ \n      \n      \n      \n      \n",
};

const cf_char_t* art_space = "  \n  \n  \n  \n  \n  \n";
const cf_char_t* art_tab = "  \n  \n  \n  \n  \n  \n";


cf_int_t cfx_art_print(const cf_char_t* input, cf_char_t* buff, cf_size_t size, cfx_art_font_t font) {
    const cf_char_t** pp = CF_NULL_PTR;
    cf_size_t i = 0, j = 0, cnt = 0, len;
    if (input == CF_NULL_PTR || buff == CF_NULL_PTR) return -1;

    len = cf_strlen(input);
    if (len == 0) return 0;

    pp = cf_malloc_z(sizeof(cf_char_t*) * len);

    /* initialize the map pointer */
    i = 0;
    while (*input) {
        if (*input == ' ') {
            pp[i] = art_space;
        } else if (*input == '\t') {
            pp[i] = art_tab;
        } else if (*input < CFX_ART_MAP_BEGIN || *input > CFX_ART_MAP_END) {
            goto CFX_ART_FAIL;
        } else {
            switch (font) {
                case CFX_ART_FONT_SLANT:
                    pp[i] = ascii_map_slant[*input - CFX_ART_MAP_BEGIN];
                    break;
                default:
                    pp[i] = ascii_map[*input - CFX_ART_MAP_BEGIN];
                    break;
            }
        }
        input++;
        i++;
    }

    /* for each line */
    i = 0;
    while (i < CFX_ART_MAX_LINES) {
        /* write one by one */
        j = 0;
        while (j < len) {
            while (*pp[j] != '\n') {
                if (cnt > size) goto CFX_ART_FAIL;
                buff[cnt++] = *pp[j];
                pp[j]++;
            }
            pp[j]++;
            j++;
        }
        buff[cnt++] = '\n';
        i++;
    }
    cf_free(pp);
    buff[cnt] = '\0';
    return cnt;

CFX_ART_FAIL:
    cf_free(pp);
    buff[0] = '\0';
    return -1;
}
