}}
// Todo: refactor (namespace opened by ChannelTypeDefinitions.h closed)
// namespace m2etis pubsub closed because of the following includes:

#include "config/GeneratedEventTypes.h"
#include "config/DetMergeConfigTest.h"

namespace m2etis {
namespace pubsub {


#include <map>
#include <vector>

#include "pubsub/filter/AttributeTypeInformation.h"
#include "pubsub/filter/AttributeAccessorBasic.h"
#include "pubsub/filter/AttributeAccessor.h"


using ::m2etis::pubsub::filter::AttributeName;
using ::m2etis::pubsub::filter::AttributeAccessor_Basic;
using ::m2etis::pubsub::filter::AttributeAccessor;


// needed for serializing attribute value accessors for filters:
// maps attribute identifier to attribute value getter
// todo: maybe move to better place
enum attribute_names {
	  POSITION_X, POSITION_Y, POSITION_REGION, BOOK_TITLE, BOOK_PRICE, BOOK_CONDITION
  };

std::map<AttributeName, std::shared_ptr<AttributeAccessor_Basic> > attributes_accessor_map = {
		{POSITION_Y, std::make_shared<AttributeAccessor<Position, int> > (
				[] (const Position& position)->int {return position.get_y();} )
		},
		{POSITION_X, std::make_shared<AttributeAccessor<Position, int> > (
				[] (const Position& position)->int {return position.get_x();} )
		},
		{POSITION_REGION, std::make_shared<AttributeAccessor<Position, std::string> > (
				[] (const Position& position)->std::string {return position.get_region();} )
		},
		{BOOK_TITLE, std::make_shared<AttributeAccessor<Book, std::string> > (
				[] (const Book& book)->std::string {return book.title_;} )
		},
		{BOOK_CONDITION, std::make_shared<AttributeAccessor<Book, std::string> > (
						[] (const Book& book)->std::string {return book.condition_;} )
		},
		{BOOK_PRICE, std::make_shared<AttributeAccessor<Book, double> > (
						[] (const Book& book)->double {return book.price_;} )
		}
};

$generated_channel_configurations

#endif
