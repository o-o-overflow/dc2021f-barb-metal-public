#
# Object, mrubyc class library
#
#  Copyright (C) 2015-2020 Kyushu Institute of Technology.
#  Copyright (C) 2015-2020 Shimane IT Open-Innovation Center.
#
#  This file is distributed under BSD 3-Clause License.
#

class Object

  # loop
  def loop
    while true
      yield
    end
  end

end
