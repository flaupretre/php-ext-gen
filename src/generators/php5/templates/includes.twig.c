
/* System include files */

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

/* config.h */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include "win32/time.h"
#elif defined(NETWARE)
#include <sys/timeval.h>
#include <sys/time.h>
#else
#include <sys/time.h>
#endif

/* PHP include files */

#include "php.h"
#include "zend_execute.h"
#include "zend_hash.h"
#include "zend_operators.h"
#include "zend_API.h"
#include "TSRM/TSRM.h"
#include "SAPI.h"
#include "php_streams.h"
#include "ext/standard/php_string.h"
#include "ext/hash/php_hash.h"
#include "ext/hash/php_hash.h"

#if ZEND_EXTENSION_API_NO >= PHP_5_5_X_API_NO
#include "zend_virtual_cwd.h"
#else
#include "TSRM/tsrm_virtual_cwd.h"
#endif

{# TODO: What is the bare minimum here ? #}
