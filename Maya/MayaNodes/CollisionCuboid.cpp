#include "stdafx.h"
#include "CollisionCuboid.h"

#include "MayaUtils/NodeTypes.h"

const MTypeId CollisionCuboid::s_TypeID( IGL_COLL_CUBOID );
const char* CollisionCuboid::s_TypeName = "igCuboid";

MStatus CollisionCuboid::Initialize()
{
  return inheritAttributesFrom( Cuboid::s_TypeName );
}