#pragma once 
auto type_collector = [](auto t_, auto predicate_, auto extractor) {
	return 0;
};

template <typename helper>
auto type_collector_t = type_collector(helper::value,
		helper::predicate,
		helper::extractor);
