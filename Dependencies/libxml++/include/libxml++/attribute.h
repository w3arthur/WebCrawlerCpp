/* attribute.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTE_H
#define __LIBXMLPP_ATTRIBUTE_H


#include "libxml++/ustring.h"

#include <libxml++/nodes/node.h>

namespace xmlpp
{

/** Represents an XML attribute node or attribute declaration.
 * This will be instantiated by the parser.
 */
class LIBXMLPP_API Attribute : public Node
{
public:
  explicit Attribute(_xmlNode* node);
  ~Attribute() override;

  /** Get the value of this attribute.
   * @returns The attribute's value.
   */
  virtual ustring get_value() const = 0;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTE_H

