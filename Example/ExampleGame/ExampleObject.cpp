//----------------------------------------------------------------------------------------------------------------------
// ExampleObject.cpp
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#include "ExampleGamePch.h"

#include "Engine/GameObjectType.h"

#include "ExampleGame/ExampleObject.h"

namespace Example
{
    HELIUM_IMPLEMENT_OBJECT( ExampleObject, ExampleGame, 0 );

    /// Constructor.
    ExampleObject::ExampleObject()
    {
    }

    /// Destructor.
    ExampleObject::~ExampleObject()
    {
    }
}
