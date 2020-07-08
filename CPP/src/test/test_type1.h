#include "type_collector.h"

struct type1 {
	static int value;
	static int predicate;
	static int extractor;
};

using fun1 = decltype(type_collector_t<type1>);
