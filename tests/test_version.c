#include "cf/version.h"
#include "cf/assert.h"

int main(int argc, char *argv[])
{
    cf_version_t ver;

    cf_assert(cf_version_from(&ver, "1.92a999") == CF_TRUE);

    cf_assert(ver.major == 1);
    cf_assert(ver.minor == 92);
    cf_assert(ver.patch == 0);
    cf_assert(ver.pretag == 'a');
    cf_assert(ver.prenum == 999);
    cf_assert(cf_version_from(&ver, "1a1234") == CF_TRUE);
    cf_assert(cf_version_from(&ver, "0.92") == CF_TRUE);
    cf_assert(cf_version_from(&ver, "1.a999") == CF_FALSE);
    cf_assert(cf_version_from(&ver, "1.a234") == CF_FALSE);
    cf_assert(cf_version_from(&ver, ".1234a234") == CF_FALSE);
    cf_assert(cf_version_from(&ver, "1.a1") == CF_FALSE);
    cf_assert(cf_version_from(&ver, "1.3c1") == CF_FALSE);
    cf_assert(cf_version_from(&ver, "2.7.2.1") == CF_FALSE);
    return 0;
}
