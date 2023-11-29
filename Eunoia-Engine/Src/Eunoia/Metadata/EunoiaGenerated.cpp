#include "../Eunoia.h"

namespace Eunoia {


	template<>
	EU_API metadata_typeid Metadata::GetTypeID < char > () { return 0; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < u8 > () { return 1; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < u16 > () { return 2; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < u32 > () { return 3; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < s8 > () { return 4; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < s16 > () { return 5; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < s32 > () { return 6; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < r32 > () { return 7; }

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < r64 > () { return 8; }

	void Metadata::InitMetadataPrimitives()
	{
		MetadataInfo info;
		info.id = 0;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "char";
		info.primitive->size = sizeof( char );
		info.primitive->type = METADATA_PRIMITIVE_CHAR;
		Metadata::RegisterMetadataInfo(info);

		info.id = 1;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "u8";
		info.primitive->size = sizeof( u8 );
		info.primitive->type = METADATA_PRIMITIVE_U8;
		Metadata::RegisterMetadataInfo(info);

		info.id = 2;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "u16";
		info.primitive->size = sizeof( u16 );
		info.primitive->type = METADATA_PRIMITIVE_U16;
		Metadata::RegisterMetadataInfo(info);

		info.id = 3;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "u32";
		info.primitive->size = sizeof( u32 );
		info.primitive->type = METADATA_PRIMITIVE_U32;
		Metadata::RegisterMetadataInfo(info);

		info.id = 4;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "s8";
		info.primitive->size = sizeof( s8 );
		info.primitive->type = METADATA_PRIMITIVE_S8;
		Metadata::RegisterMetadataInfo(info);

		info.id = 5;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "s16";
		info.primitive->size = sizeof( s16 );
		info.primitive->type = METADATA_PRIMITIVE_S16;
		Metadata::RegisterMetadataInfo(info);

		info.id = 6;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "s32";
		info.primitive->size = sizeof( s32 );
		info.primitive->type = METADATA_PRIMITIVE_S32;
		Metadata::RegisterMetadataInfo(info);

		info.id = 7;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "r32";
		info.primitive->size = sizeof( r32 );
		info.primitive->type = METADATA_PRIMITIVE_R32;
		Metadata::RegisterMetadataInfo(info);

		info.id = 8;
		info.type = METADATA_PRIMITIVE;
		info.primitive = new MetadataPrimitive();
		info.primitive->name = "r64";
		info.primitive->size = sizeof( r64 );
		info.primitive->type = METADATA_PRIMITIVE_R64;
		Metadata::RegisterMetadataInfo(info);
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < v2 > () { return 9; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<v2>()
	{
		MetadataInfo info;
		info.id = 9;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "v2";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( v2 );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< v2 >;
		info.cls->members.SetCapacityAndElementCount( 2 );

		info.cls->members[ 0 ].name = "x";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( v2, v2::x );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "y";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( v2, v2::y );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < v3 > () { return 10; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<v3>()
	{
		MetadataInfo info;
		info.id = 10;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "v3";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( v3 );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< v3 >;
		info.cls->members.SetCapacityAndElementCount( 3 );

		info.cls->members[ 0 ].name = "x";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( v3, v3::x );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "y";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( v3, v3::y );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "z";
		info.cls->members[ 2 ].typeName = "r32";
		info.cls->members[ 2 ].typeID = GetTypeID<r32>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( v3, v3::z );
		info.cls->members[ 2 ].size = sizeof( r32 );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < v4 > () { return 11; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<v4>()
	{
		MetadataInfo info;
		info.id = 11;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "v4";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( v4 );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< v4 >;
		info.cls->members.SetCapacityAndElementCount( 4 );

		info.cls->members[ 0 ].name = "x";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( v4, v4::x );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "y";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( v4, v4::y );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "z";
		info.cls->members[ 2 ].typeName = "r32";
		info.cls->members[ 2 ].typeID = GetTypeID<r32>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( v4, v4::z );
		info.cls->members[ 2 ].size = sizeof( r32 );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		info.cls->members[ 3 ].name = "w";
		info.cls->members[ 3 ].typeName = "r32";
		info.cls->members[ 3 ].typeID = GetTypeID<r32>();
		info.cls->members[ 3 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 3 ].offset = offsetof( v4, v4::w );
		info.cls->members[ 3 ].size = sizeof( r32 );
		info.cls->members[ 3 ].isStatic = false;
		info.cls->members[ 3 ].isConst = false;
		info.cls->members[ 3 ].isPointer = false;
		info.cls->members[ 3 ].arrayLength = 1;
		info.cls->members[ 3 ].uiSliderMin = v4(0.0);
		info.cls->members[ 3 ].uiSliderMax = v4(0.0);
		info.cls->members[ 3 ].uiSliderSpeed = 0.1;
		info.cls->members[ 3 ].is32BitBool = false;
		info.cls->members[ 3 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < m3 > () { return 12; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<m3>()
	{
		MetadataInfo info;
		info.id = 12;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "m3";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( m3 );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< m3 >;
		info.cls->members.SetCapacityAndElementCount( 0 );

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < m4 > () { return 13; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<m4>()
	{
		MetadataInfo info;
		info.id = 13;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "m4";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( m4 );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< m4 >;
		info.cls->members.SetCapacityAndElementCount( 0 );

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < quat > () { return 14; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<quat>()
	{
		MetadataInfo info;
		info.id = 14;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "quat";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( quat );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< quat >;
		info.cls->members.SetCapacityAndElementCount( 4 );

		info.cls->members[ 0 ].name = "x";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( quat, quat::x );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "y";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( quat, quat::y );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "z";
		info.cls->members[ 2 ].typeName = "r32";
		info.cls->members[ 2 ].typeID = GetTypeID<r32>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( quat, quat::z );
		info.cls->members[ 2 ].size = sizeof( r32 );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		info.cls->members[ 3 ].name = "w";
		info.cls->members[ 3 ].typeName = "r32";
		info.cls->members[ 3 ].typeID = GetTypeID<r32>();
		info.cls->members[ 3 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 3 ].offset = offsetof( quat, quat::w );
		info.cls->members[ 3 ].size = sizeof( r32 );
		info.cls->members[ 3 ].isStatic = false;
		info.cls->members[ 3 ].isConst = false;
		info.cls->members[ 3 ].isPointer = false;
		info.cls->members[ 3 ].arrayLength = 1;
		info.cls->members[ 3 ].uiSliderMin = v4(0.0);
		info.cls->members[ 3 ].uiSliderMax = v4(0.0);
		info.cls->members[ 3 ].uiSliderSpeed = 0.1;
		info.cls->members[ 3 ].is32BitBool = false;
		info.cls->members[ 3 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Transform2D > () { return 15; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Transform2D>()
	{
		MetadataInfo info;
		info.id = 15;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Transform2D";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( Transform2D );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Transform2D >;
		info.cls->members.SetCapacityAndElementCount( 3 );

		info.cls->members[ 0 ].name = "pos";
		info.cls->members[ 0 ].typeName = "v2";
		info.cls->members[ 0 ].typeID = GetTypeID<v2>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Transform2D, Transform2D::pos );
		info.cls->members[ 0 ].size = sizeof( v2 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "scale";
		info.cls->members[ 1 ].typeName = "v2";
		info.cls->members[ 1 ].typeID = GetTypeID<v2>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Transform2D, Transform2D::scale );
		info.cls->members[ 1 ].size = sizeof( v2 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "rot";
		info.cls->members[ 2 ].typeName = "r32";
		info.cls->members[ 2 ].typeID = GetTypeID<r32>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( Transform2D, Transform2D::rot );
		info.cls->members[ 2 ].size = sizeof( r32 );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Transform3D > () { return 16; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Transform3D>()
	{
		MetadataInfo info;
		info.id = 16;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Transform3D";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( Transform3D );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Transform3D >;
		info.cls->members.SetCapacityAndElementCount( 3 );

		info.cls->members[ 0 ].name = "pos";
		info.cls->members[ 0 ].typeName = "v3";
		info.cls->members[ 0 ].typeID = GetTypeID<v3>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Transform3D, Transform3D::pos );
		info.cls->members[ 0 ].size = sizeof( v3 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "scale";
		info.cls->members[ 1 ].typeName = "v3";
		info.cls->members[ 1 ].typeID = GetTypeID<v3>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Transform3D, Transform3D::scale );
		info.cls->members[ 1 ].size = sizeof( v3 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "rot";
		info.cls->members[ 2 ].typeName = "quat";
		info.cls->members[ 2 ].typeID = GetTypeID<quat>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( Transform3D, Transform3D::rot );
		info.cls->members[ 2 ].size = sizeof( quat );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Gamepad > () { return 17; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< Gamepad >()
	{
		MetadataInfo info;
		info.id = 17;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "Gamepad";
		info.enm->values.SetCapacityAndElementCount( 7 );
		info.enm->values[ 0 ].name = "EU_GAMEPAD_0";
		info.enm->values[ 0 ].value = EU_GAMEPAD_0;
		info.enm->values[ 1 ].name = "EU_GAMEPAD_1";
		info.enm->values[ 1 ].value = EU_GAMEPAD_1;
		info.enm->values[ 2 ].name = "EU_GAMEPAD_2";
		info.enm->values[ 2 ].value = EU_GAMEPAD_2;
		info.enm->values[ 3 ].name = "EU_GAMEPAD_3";
		info.enm->values[ 3 ].value = EU_GAMEPAD_3;
		info.enm->values[ 4 ].name = "EU_NUM_GAMEPADS";
		info.enm->values[ 4 ].value = EU_NUM_GAMEPADS;
		info.enm->values[ 5 ].name = "EU_FIRST_GAMEPAD";
		info.enm->values[ 5 ].value = EU_FIRST_GAMEPAD;
		info.enm->values[ 6 ].name = "EU_LAST_GAMEPAD";
		info.enm->values[ 6 ].value = EU_LAST_GAMEPAD;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Key > () { return 18; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< Key >()
	{
		MetadataInfo info;
		info.id = 18;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "Key";
		info.enm->values.SetCapacityAndElementCount( 93 );
		info.enm->values[ 0 ].name = "EU_KEY_ERROR";
		info.enm->values[ 0 ].value = EU_KEY_ERROR;
		info.enm->values[ 1 ].name = "EU_KEY_ESC";
		info.enm->values[ 1 ].value = EU_KEY_ESC;
		info.enm->values[ 2 ].name = "EU_KEY_1";
		info.enm->values[ 2 ].value = EU_KEY_1;
		info.enm->values[ 3 ].name = "EU_KEY_2";
		info.enm->values[ 3 ].value = EU_KEY_2;
		info.enm->values[ 4 ].name = "EU_KEY_3";
		info.enm->values[ 4 ].value = EU_KEY_3;
		info.enm->values[ 5 ].name = "EU_KEY_4";
		info.enm->values[ 5 ].value = EU_KEY_4;
		info.enm->values[ 6 ].name = "EU_KEY_5";
		info.enm->values[ 6 ].value = EU_KEY_5;
		info.enm->values[ 7 ].name = "EU_KEY_6";
		info.enm->values[ 7 ].value = EU_KEY_6;
		info.enm->values[ 8 ].name = "EU_KEY_7";
		info.enm->values[ 8 ].value = EU_KEY_7;
		info.enm->values[ 9 ].name = "EU_KEY_8";
		info.enm->values[ 9 ].value = EU_KEY_8;
		info.enm->values[ 10 ].name = "EU_KEY_9";
		info.enm->values[ 10 ].value = EU_KEY_9;
		info.enm->values[ 11 ].name = "EU_KEY_0";
		info.enm->values[ 11 ].value = EU_KEY_0;
		info.enm->values[ 12 ].name = "EU_KEY_DASH";
		info.enm->values[ 12 ].value = EU_KEY_DASH;
		info.enm->values[ 13 ].name = "EU_KEY_EQUALS";
		info.enm->values[ 13 ].value = EU_KEY_EQUALS;
		info.enm->values[ 14 ].name = "EU_KEY_BACKSPACE";
		info.enm->values[ 14 ].value = EU_KEY_BACKSPACE;
		info.enm->values[ 15 ].name = "EU_KEY_TAB";
		info.enm->values[ 15 ].value = EU_KEY_TAB;
		info.enm->values[ 16 ].name = "EU_KEY_Q";
		info.enm->values[ 16 ].value = EU_KEY_Q;
		info.enm->values[ 17 ].name = "EU_KEY_W";
		info.enm->values[ 17 ].value = EU_KEY_W;
		info.enm->values[ 18 ].name = "EU_KEY_E";
		info.enm->values[ 18 ].value = EU_KEY_E;
		info.enm->values[ 19 ].name = "EU_KEY_R";
		info.enm->values[ 19 ].value = EU_KEY_R;
		info.enm->values[ 20 ].name = "EU_KEY_T";
		info.enm->values[ 20 ].value = EU_KEY_T;
		info.enm->values[ 21 ].name = "EU_KEY_Y";
		info.enm->values[ 21 ].value = EU_KEY_Y;
		info.enm->values[ 22 ].name = "EU_KEY_U";
		info.enm->values[ 22 ].value = EU_KEY_U;
		info.enm->values[ 23 ].name = "EU_KEY_I";
		info.enm->values[ 23 ].value = EU_KEY_I;
		info.enm->values[ 24 ].name = "EU_KEY_O";
		info.enm->values[ 24 ].value = EU_KEY_O;
		info.enm->values[ 25 ].name = "EU_KEY_P";
		info.enm->values[ 25 ].value = EU_KEY_P;
		info.enm->values[ 26 ].name = "EU_KEY_LEFT_BRACKET";
		info.enm->values[ 26 ].value = EU_KEY_LEFT_BRACKET;
		info.enm->values[ 27 ].name = "EU_KEY_RIGHT_BRACKET";
		info.enm->values[ 27 ].value = EU_KEY_RIGHT_BRACKET;
		info.enm->values[ 28 ].name = "EU_KEY_ENTER";
		info.enm->values[ 28 ].value = EU_KEY_ENTER;
		info.enm->values[ 29 ].name = "EU_KEY_CTL";
		info.enm->values[ 29 ].value = EU_KEY_CTL;
		info.enm->values[ 30 ].name = "EU_KEY_A";
		info.enm->values[ 30 ].value = EU_KEY_A;
		info.enm->values[ 31 ].name = "EU_KEY_S";
		info.enm->values[ 31 ].value = EU_KEY_S;
		info.enm->values[ 32 ].name = "EU_KEY_D";
		info.enm->values[ 32 ].value = EU_KEY_D;
		info.enm->values[ 33 ].name = "EU_KEY_F";
		info.enm->values[ 33 ].value = EU_KEY_F;
		info.enm->values[ 34 ].name = "EU_KEY_G";
		info.enm->values[ 34 ].value = EU_KEY_G;
		info.enm->values[ 35 ].name = "EU_KEY_H";
		info.enm->values[ 35 ].value = EU_KEY_H;
		info.enm->values[ 36 ].name = "EU_KEY_J";
		info.enm->values[ 36 ].value = EU_KEY_J;
		info.enm->values[ 37 ].name = "EU_KEY_K";
		info.enm->values[ 37 ].value = EU_KEY_K;
		info.enm->values[ 38 ].name = "EU_KEY_L";
		info.enm->values[ 38 ].value = EU_KEY_L;
		info.enm->values[ 39 ].name = "EU_KEY_SEMI_COLON";
		info.enm->values[ 39 ].value = EU_KEY_SEMI_COLON;
		info.enm->values[ 40 ].name = "EU_KEY_QUOTE";
		info.enm->values[ 40 ].value = EU_KEY_QUOTE;
		info.enm->values[ 41 ].name = "EU_KEY_TILDE";
		info.enm->values[ 41 ].value = EU_KEY_TILDE;
		info.enm->values[ 42 ].name = "EU_KEY_LEFT_SHIFT";
		info.enm->values[ 42 ].value = EU_KEY_LEFT_SHIFT;
		info.enm->values[ 43 ].name = "EU_KEY_BACK_SLASH";
		info.enm->values[ 43 ].value = EU_KEY_BACK_SLASH;
		info.enm->values[ 44 ].name = "EU_KEY_Z";
		info.enm->values[ 44 ].value = EU_KEY_Z;
		info.enm->values[ 45 ].name = "EU_KEY_X";
		info.enm->values[ 45 ].value = EU_KEY_X;
		info.enm->values[ 46 ].name = "EU_KEY_C";
		info.enm->values[ 46 ].value = EU_KEY_C;
		info.enm->values[ 47 ].name = "EU_KEY_V";
		info.enm->values[ 47 ].value = EU_KEY_V;
		info.enm->values[ 48 ].name = "EU_KEY_B";
		info.enm->values[ 48 ].value = EU_KEY_B;
		info.enm->values[ 49 ].name = "EU_KEY_N";
		info.enm->values[ 49 ].value = EU_KEY_N;
		info.enm->values[ 50 ].name = "EU_KEY_M";
		info.enm->values[ 50 ].value = EU_KEY_M;
		info.enm->values[ 51 ].name = "EU_KEY_COMMA";
		info.enm->values[ 51 ].value = EU_KEY_COMMA;
		info.enm->values[ 52 ].name = "EU_KEY_PERIOD";
		info.enm->values[ 52 ].value = EU_KEY_PERIOD;
		info.enm->values[ 53 ].name = "EU_KEY_SLASH";
		info.enm->values[ 53 ].value = EU_KEY_SLASH;
		info.enm->values[ 54 ].name = "EU_KEY_RIGHT_SHIFT";
		info.enm->values[ 54 ].value = EU_KEY_RIGHT_SHIFT;
		info.enm->values[ 55 ].name = "EU_KEY_KP_ASTERISK";
		info.enm->values[ 55 ].value = EU_KEY_KP_ASTERISK;
		info.enm->values[ 56 ].name = "EU_KEY_LEFT_ALT";
		info.enm->values[ 56 ].value = EU_KEY_LEFT_ALT;
		info.enm->values[ 57 ].name = "EU_KEY_SPACE";
		info.enm->values[ 57 ].value = EU_KEY_SPACE;
		info.enm->values[ 58 ].name = "EU_KEY_CAPS_LOCK";
		info.enm->values[ 58 ].value = EU_KEY_CAPS_LOCK;
		info.enm->values[ 59 ].name = "EU_KEY_F1";
		info.enm->values[ 59 ].value = EU_KEY_F1;
		info.enm->values[ 60 ].name = "EU_KEY_F2";
		info.enm->values[ 60 ].value = EU_KEY_F2;
		info.enm->values[ 61 ].name = "EU_KEY_F3";
		info.enm->values[ 61 ].value = EU_KEY_F3;
		info.enm->values[ 62 ].name = "EU_KEY_F4";
		info.enm->values[ 62 ].value = EU_KEY_F4;
		info.enm->values[ 63 ].name = "EU_KEY_F5";
		info.enm->values[ 63 ].value = EU_KEY_F5;
		info.enm->values[ 64 ].name = "EU_KEY_F6";
		info.enm->values[ 64 ].value = EU_KEY_F6;
		info.enm->values[ 65 ].name = "EU_KEY_F7";
		info.enm->values[ 65 ].value = EU_KEY_F7;
		info.enm->values[ 66 ].name = "EU_KEY_F8";
		info.enm->values[ 66 ].value = EU_KEY_F8;
		info.enm->values[ 67 ].name = "EU_KEY_F9";
		info.enm->values[ 67 ].value = EU_KEY_F9;
		info.enm->values[ 68 ].name = "EU_KEY_F10";
		info.enm->values[ 68 ].value = EU_KEY_F10;
		info.enm->values[ 69 ].name = "EU_KEY_NUM_LOCK";
		info.enm->values[ 69 ].value = EU_KEY_NUM_LOCK;
		info.enm->values[ 70 ].name = "EU_KEY_SCROLL_LOCK";
		info.enm->values[ 70 ].value = EU_KEY_SCROLL_LOCK;
		info.enm->values[ 71 ].name = "EU_KEY_KP_7";
		info.enm->values[ 71 ].value = EU_KEY_KP_7;
		info.enm->values[ 72 ].name = "EU_KEY_KP_8";
		info.enm->values[ 72 ].value = EU_KEY_KP_8;
		info.enm->values[ 73 ].name = "EU_KEY_KP_9";
		info.enm->values[ 73 ].value = EU_KEY_KP_9;
		info.enm->values[ 74 ].name = "EU_KEY_KP_MINUS";
		info.enm->values[ 74 ].value = EU_KEY_KP_MINUS;
		info.enm->values[ 75 ].name = "EU_KEY_KP_4";
		info.enm->values[ 75 ].value = EU_KEY_KP_4;
		info.enm->values[ 76 ].name = "EU_KEY_KP_5";
		info.enm->values[ 76 ].value = EU_KEY_KP_5;
		info.enm->values[ 77 ].name = "EU_KEY_KP_6";
		info.enm->values[ 77 ].value = EU_KEY_KP_6;
		info.enm->values[ 78 ].name = "EU_KEY_KP_PLUS";
		info.enm->values[ 78 ].value = EU_KEY_KP_PLUS;
		info.enm->values[ 79 ].name = "EU_KEY_KP_1";
		info.enm->values[ 79 ].value = EU_KEY_KP_1;
		info.enm->values[ 80 ].name = "EU_KEY_KP_2";
		info.enm->values[ 80 ].value = EU_KEY_KP_2;
		info.enm->values[ 81 ].name = "EU_KEY_KP_3";
		info.enm->values[ 81 ].value = EU_KEY_KP_3;
		info.enm->values[ 82 ].name = "EU_KEY_KP_0";
		info.enm->values[ 82 ].value = EU_KEY_KP_0;
		info.enm->values[ 83 ].name = "EU_KEY_KP_DEL";
		info.enm->values[ 83 ].value = EU_KEY_KP_DEL;
		info.enm->values[ 84 ].name = "EU_KEY_SYS_RQ";
		info.enm->values[ 84 ].value = EU_KEY_SYS_RQ;
		info.enm->values[ 85 ].name = "EU_KEY_BLANK";
		info.enm->values[ 85 ].value = EU_KEY_BLANK;
		info.enm->values[ 86 ].name = "EU_KEY_F11";
		info.enm->values[ 86 ].value = EU_KEY_F11;
		info.enm->values[ 87 ].name = "EU_KEY_F12";
		info.enm->values[ 87 ].value = EU_KEY_F12;
		info.enm->values[ 88 ].name = "EU_KEY_UP";
		info.enm->values[ 88 ].value = EU_KEY_UP;
		info.enm->values[ 89 ].name = "EU_KEY_DOWN";
		info.enm->values[ 89 ].value = EU_KEY_DOWN;
		info.enm->values[ 90 ].name = "EU_KEY_LEFT";
		info.enm->values[ 90 ].value = EU_KEY_LEFT;
		info.enm->values[ 91 ].name = "EU_KEY_RIGHT";
		info.enm->values[ 91 ].value = EU_KEY_RIGHT;
		info.enm->values[ 92 ].name = "EU_NUM_KEYS";
		info.enm->values[ 92 ].value = EU_NUM_KEYS;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < MouseButton > () { return 19; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< MouseButton >()
	{
		MetadataInfo info;
		info.id = 19;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "MouseButton";
		info.enm->values.SetCapacityAndElementCount( 7 );
		info.enm->values[ 0 ].name = "EU_BUTTON_LEFT";
		info.enm->values[ 0 ].value = EU_BUTTON_LEFT;
		info.enm->values[ 1 ].name = "EU_BUTTON_RIGHT";
		info.enm->values[ 1 ].value = EU_BUTTON_RIGHT;
		info.enm->values[ 2 ].name = "EU_BUTTON_EXTRA_1";
		info.enm->values[ 2 ].value = EU_BUTTON_EXTRA_1;
		info.enm->values[ 3 ].name = "EU_BUTTON_EXTRA_2";
		info.enm->values[ 3 ].value = EU_BUTTON_EXTRA_2;
		info.enm->values[ 4 ].name = "EU_BUTTON_EXTRA_3";
		info.enm->values[ 4 ].value = EU_BUTTON_EXTRA_3;
		info.enm->values[ 5 ].name = "EU_MOUSE_WHEEL";
		info.enm->values[ 5 ].value = EU_MOUSE_WHEEL;
		info.enm->values[ 6 ].name = "EU_NUM_MOUSE_BUTTONS";
		info.enm->values[ 6 ].value = EU_NUM_MOUSE_BUTTONS;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < GamepadButton > () { return 20; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< GamepadButton >()
	{
		MetadataInfo info;
		info.id = 20;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "GamepadButton";
		info.enm->values.SetCapacityAndElementCount( 19 );
		info.enm->values[ 0 ].name = "EU_GAMEPAD_XBOX360_BUTTON_DPAD_UP";
		info.enm->values[ 0 ].value = EU_GAMEPAD_XBOX360_BUTTON_DPAD_UP;
		info.enm->values[ 1 ].name = "EU_GAMEPAD_XBOX360_BUTTON_DPAD_DOWN";
		info.enm->values[ 1 ].value = EU_GAMEPAD_XBOX360_BUTTON_DPAD_DOWN;
		info.enm->values[ 2 ].name = "EU_GAMEPAD_XBOX360_BUTTON_DPAD_LEFT";
		info.enm->values[ 2 ].value = EU_GAMEPAD_XBOX360_BUTTON_DPAD_LEFT;
		info.enm->values[ 3 ].name = "EU_GAMEPAD_XBOX360_BUTTON_DPAD_RIGHT";
		info.enm->values[ 3 ].value = EU_GAMEPAD_XBOX360_BUTTON_DPAD_RIGHT;
		info.enm->values[ 4 ].name = "EU_GAMEPAD_XBOX360_BUTTON_START";
		info.enm->values[ 4 ].value = EU_GAMEPAD_XBOX360_BUTTON_START;
		info.enm->values[ 5 ].name = "EU_GAMEPAD_XBOX360_BUTTON_BACK";
		info.enm->values[ 5 ].value = EU_GAMEPAD_XBOX360_BUTTON_BACK;
		info.enm->values[ 6 ].name = "EU_GAMEPAD_XBOX360_BUTTON_LEFT_THUMB";
		info.enm->values[ 6 ].value = EU_GAMEPAD_XBOX360_BUTTON_LEFT_THUMB;
		info.enm->values[ 7 ].name = "EU_GAMEPAD_XBOX360_BUTTON_RIGHT_THUMB";
		info.enm->values[ 7 ].value = EU_GAMEPAD_XBOX360_BUTTON_RIGHT_THUMB;
		info.enm->values[ 8 ].name = "EU_GAMEPAD_XBOX360_BUTTON_LEFT_SHOULDER";
		info.enm->values[ 8 ].value = EU_GAMEPAD_XBOX360_BUTTON_LEFT_SHOULDER;
		info.enm->values[ 9 ].name = "EU_GAMEPAD_XBOX360_BUTTON_RIGHT_SHOULDER";
		info.enm->values[ 9 ].value = EU_GAMEPAD_XBOX360_BUTTON_RIGHT_SHOULDER;
		info.enm->values[ 10 ].name = "EU_GAMEPAD_XBOX360_BUTTON_A";
		info.enm->values[ 10 ].value = EU_GAMEPAD_XBOX360_BUTTON_A;
		info.enm->values[ 11 ].name = "EU_GAMEPAD_XBOX360_BUTTON_B";
		info.enm->values[ 11 ].value = EU_GAMEPAD_XBOX360_BUTTON_B;
		info.enm->values[ 12 ].name = "EU_GAMEPAD_XBOX360_BUTTON_X";
		info.enm->values[ 12 ].value = EU_GAMEPAD_XBOX360_BUTTON_X;
		info.enm->values[ 13 ].name = "EU_GAMEPAD_XBOX360_BUTTON_Y";
		info.enm->values[ 13 ].value = EU_GAMEPAD_XBOX360_BUTTON_Y;
		info.enm->values[ 14 ].name = "EU_GAMEPAD_XBOX360_BUTTON_FIRST_BUTTON";
		info.enm->values[ 14 ].value = EU_GAMEPAD_XBOX360_BUTTON_FIRST_BUTTON;
		info.enm->values[ 15 ].name = "EU_GAMEPAD_XBOX360_BUTTON_LAST_BUTTON";
		info.enm->values[ 15 ].value = EU_GAMEPAD_XBOX360_BUTTON_LAST_BUTTON;
		info.enm->values[ 16 ].name = "EU_GAMEPAD_XBOX360_BUTTON_LT";
		info.enm->values[ 16 ].value = EU_GAMEPAD_XBOX360_BUTTON_LT;
		info.enm->values[ 17 ].name = "EU_GAMEPAD_XBOX360_BUTTON_RT";
		info.enm->values[ 17 ].value = EU_GAMEPAD_XBOX360_BUTTON_RT;
		info.enm->values[ 18 ].name = "EU_NUM_GAMEPAD_BUTTONS";
		info.enm->values[ 18 ].value = EU_NUM_GAMEPAD_BUTTONS;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < GamepadTrigger > () { return 21; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< GamepadTrigger >()
	{
		MetadataInfo info;
		info.id = 21;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "GamepadTrigger";
		info.enm->values.SetCapacityAndElementCount( 3 );
		info.enm->values[ 0 ].name = "EU_GAMEPAD_XBOX360_TRIGGER_LT";
		info.enm->values[ 0 ].value = EU_GAMEPAD_XBOX360_TRIGGER_LT;
		info.enm->values[ 1 ].name = "EU_GAMEPAD_XBOX360_TRIGGER_RT";
		info.enm->values[ 1 ].value = EU_GAMEPAD_XBOX360_TRIGGER_RT;
		info.enm->values[ 2 ].name = "EU_NUM_GAMEPAD_TRIGGERS";
		info.enm->values[ 2 ].value = EU_NUM_GAMEPAD_TRIGGERS;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < GamepadThumbstick > () { return 22; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< GamepadThumbstick >()
	{
		MetadataInfo info;
		info.id = 22;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "GamepadThumbstick";
		info.enm->values.SetCapacityAndElementCount( 3 );
		info.enm->values[ 0 ].name = "EU_GAMEPAD_XBOX360_THUMBSTICK_LEFT";
		info.enm->values[ 0 ].value = EU_GAMEPAD_XBOX360_THUMBSTICK_LEFT;
		info.enm->values[ 1 ].name = "EU_GAMEPAD_XBOX360_THUMBSTICK_RIGHT";
		info.enm->values[ 1 ].value = EU_GAMEPAD_XBOX360_THUMBSTICK_RIGHT;
		info.enm->values[ 2 ].name = "EU_NUM_GAMEPAD_THUMBSTICKS";
		info.enm->values[ 2 ].value = EU_NUM_GAMEPAD_THUMBSTICKS;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Transform2DComponent > () { return 23; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Transform2DComponent>()
	{
		MetadataInfo info;
		info.id = 23;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Transform2DComponent";
		info.cls->baseClassName = "ECSComponent";
		info.cls->baseClassSize = sizeof( ECSComponent );
		info.cls->size = sizeof( Transform2DComponent );
		info.cls->isComponent = true;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Transform2DComponent >;
		info.cls->members.SetCapacityAndElementCount( 2 );

		info.cls->members[ 0 ].name = "worldTransform";
		info.cls->members[ 0 ].typeName = "Transform2D";
		info.cls->members[ 0 ].typeID = GetTypeID<Transform2D>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Transform2DComponent, Transform2DComponent::worldTransform );
		info.cls->members[ 0 ].size = sizeof( Transform2D );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = true;

		info.cls->members[ 1 ].name = "localTransform";
		info.cls->members[ 1 ].typeName = "Transform2D";
		info.cls->members[ 1 ].typeID = GetTypeID<Transform2D>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Transform2DComponent, Transform2DComponent::localTransform );
		info.cls->members[ 1 ].size = sizeof( Transform2D );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < SpriteComponent > () { return 24; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<SpriteComponent>()
	{
		MetadataInfo info;
		info.id = 24;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "SpriteComponent";
		info.cls->baseClassName = "ECSComponent";
		info.cls->baseClassSize = sizeof( ECSComponent );
		info.cls->size = sizeof( SpriteComponent );
		info.cls->isComponent = true;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< SpriteComponent >;
		info.cls->members.SetCapacityAndElementCount( 4 );

		info.cls->members[ 0 ].name = "size";
		info.cls->members[ 0 ].typeName = "v2";
		info.cls->members[ 0 ].typeID = GetTypeID<v2>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( SpriteComponent, SpriteComponent::size );
		info.cls->members[ 0 ].size = sizeof( v2 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "color";
		info.cls->members[ 1 ].typeName = "v4";
		info.cls->members[ 1 ].typeID = GetTypeID<v4>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( SpriteComponent, SpriteComponent::color );
		info.cls->members[ 1 ].size = sizeof( v4 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "texture";
		info.cls->members[ 2 ].typeName = "TextureID";
		info.cls->members[ 2 ].typeID = GetTypeID<TextureID>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( SpriteComponent, SpriteComponent::texture );
		info.cls->members[ 2 ].size = sizeof( TextureID );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		info.cls->members[ 3 ].name = "occluder";
		info.cls->members[ 3 ].typeName = "b32";
		info.cls->members[ 3 ].typeID = GetTypeID<b32>();
		info.cls->members[ 3 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 3 ].offset = offsetof( SpriteComponent, SpriteComponent::occluder );
		info.cls->members[ 3 ].size = sizeof( b32 );
		info.cls->members[ 3 ].isStatic = false;
		info.cls->members[ 3 ].isConst = false;
		info.cls->members[ 3 ].isPointer = false;
		info.cls->members[ 3 ].arrayLength = 1;
		info.cls->members[ 3 ].uiSliderMin = v4(0.0);
		info.cls->members[ 3 ].uiSliderMax = v4(0.0);
		info.cls->members[ 3 ].uiSliderSpeed = 0.1;
		info.cls->members[ 3 ].is32BitBool = true;
		info.cls->members[ 3 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Camera2DOrthoType > () { return 25; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo< Camera2DOrthoType >()
	{
		MetadataInfo info;
		info.id = 25;
		info.type = METADATA_ENUM;
		info.enm = Eunoia::Metadata::AllocateEnum( true );
		info.enm->name = "Camera2DOrthoType";
		info.enm->values.SetCapacityAndElementCount( 2 );
		info.enm->values[ 0 ].name = "EU_CAMERA_2D_STANDARD_ORTHO";
		info.enm->values[ 0 ].value = EU_CAMERA_2D_STANDARD_ORTHO;
		info.enm->values[ 1 ].name = "EU_CAMERA_2D_ADJUSTED_ORTHO";
		info.enm->values[ 1 ].value = EU_CAMERA_2D_ADJUSTED_ORTHO;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Camera2DStandardOrtho > () { return 26; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Camera2DStandardOrtho>()
	{
		MetadataInfo info;
		info.id = 26;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Camera2DStandardOrtho";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( Camera2DStandardOrtho );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Camera2DStandardOrtho >;
		info.cls->members.SetCapacityAndElementCount( 4 );

		info.cls->members[ 0 ].name = "left";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Camera2DStandardOrtho, Camera2DStandardOrtho::left );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "right";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Camera2DStandardOrtho, Camera2DStandardOrtho::right );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "bottom";
		info.cls->members[ 2 ].typeName = "r32";
		info.cls->members[ 2 ].typeID = GetTypeID<r32>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( Camera2DStandardOrtho, Camera2DStandardOrtho::bottom );
		info.cls->members[ 2 ].size = sizeof( r32 );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		info.cls->members[ 3 ].name = "top";
		info.cls->members[ 3 ].typeName = "r32";
		info.cls->members[ 3 ].typeID = GetTypeID<r32>();
		info.cls->members[ 3 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 3 ].offset = offsetof( Camera2DStandardOrtho, Camera2DStandardOrtho::top );
		info.cls->members[ 3 ].size = sizeof( r32 );
		info.cls->members[ 3 ].isStatic = false;
		info.cls->members[ 3 ].isConst = false;
		info.cls->members[ 3 ].isPointer = false;
		info.cls->members[ 3 ].arrayLength = 1;
		info.cls->members[ 3 ].uiSliderMin = v4(0.0);
		info.cls->members[ 3 ].uiSliderMax = v4(0.0);
		info.cls->members[ 3 ].uiSliderSpeed = 0.1;
		info.cls->members[ 3 ].is32BitBool = false;
		info.cls->members[ 3 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Camera2DAdjustedOrtho > () { return 27; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Camera2DAdjustedOrtho>()
	{
		MetadataInfo info;
		info.id = 27;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Camera2DAdjustedOrtho";
		info.cls->baseClassName = "";
		info.cls->baseClassSize = 0;
		info.cls->size = sizeof( Camera2DAdjustedOrtho );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Camera2DAdjustedOrtho >;
		info.cls->members.SetCapacityAndElementCount( 2 );

		info.cls->members[ 0 ].name = "orthoScale";
		info.cls->members[ 0 ].typeName = "r32";
		info.cls->members[ 0 ].typeID = GetTypeID<r32>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Camera2DAdjustedOrtho, Camera2DAdjustedOrtho::orthoScale );
		info.cls->members[ 0 ].size = sizeof( r32 );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "orthoAdjusted";
		info.cls->members[ 1 ].typeName = "r32";
		info.cls->members[ 1 ].typeID = GetTypeID<r32>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Camera2DAdjustedOrtho, Camera2DAdjustedOrtho::orthoAdjusted );
		info.cls->members[ 1 ].size = sizeof( r32 );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < Camera2DComponent > () { return 28; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<Camera2DComponent>()
	{
		MetadataInfo info;
		info.id = 28;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "Camera2DComponent";
		info.cls->baseClassName = "ECSComponent";
		info.cls->baseClassSize = sizeof( ECSComponent );
		info.cls->size = sizeof( Camera2DComponent );
		info.cls->isComponent = true;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< Camera2DComponent >;
		info.cls->members.SetCapacityAndElementCount( 3 );

		info.cls->members[ 0 ].name = "type";
		info.cls->members[ 0 ].typeName = "Camera2DOrthoType";
		info.cls->members[ 0 ].typeID = GetTypeID<Camera2DOrthoType>();
		info.cls->members[ 0 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 0 ].offset = offsetof( Camera2DComponent, Camera2DComponent::type );
		info.cls->members[ 0 ].size = sizeof( Camera2DOrthoType );
		info.cls->members[ 0 ].isStatic = false;
		info.cls->members[ 0 ].isConst = false;
		info.cls->members[ 0 ].isPointer = false;
		info.cls->members[ 0 ].arrayLength = 1;
		info.cls->members[ 0 ].uiSliderMin = v4(0.0);
		info.cls->members[ 0 ].uiSliderMax = v4(0.0);
		info.cls->members[ 0 ].uiSliderSpeed = 0.1;
		info.cls->members[ 0 ].is32BitBool = false;
		info.cls->members[ 0 ].isNoEdit = false;

		info.cls->members[ 1 ].name = "standard";
		info.cls->members[ 1 ].typeName = "Camera2DStandardOrtho";
		info.cls->members[ 1 ].typeID = GetTypeID<Camera2DStandardOrtho>();
		info.cls->members[ 1 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 1 ].offset = offsetof( Camera2DComponent, Camera2DComponent::standard );
		info.cls->members[ 1 ].size = sizeof( Camera2DStandardOrtho );
		info.cls->members[ 1 ].isStatic = false;
		info.cls->members[ 1 ].isConst = false;
		info.cls->members[ 1 ].isPointer = false;
		info.cls->members[ 1 ].arrayLength = 1;
		info.cls->members[ 1 ].uiSliderMin = v4(0.0);
		info.cls->members[ 1 ].uiSliderMax = v4(0.0);
		info.cls->members[ 1 ].uiSliderSpeed = 0.1;
		info.cls->members[ 1 ].is32BitBool = false;
		info.cls->members[ 1 ].isNoEdit = false;

		info.cls->members[ 2 ].name = "adjusted";
		info.cls->members[ 2 ].typeName = "Camera2DAdjustedOrtho";
		info.cls->members[ 2 ].typeID = GetTypeID<Camera2DAdjustedOrtho>();
		info.cls->members[ 2 ].accessModifier = METADATA_ACCESS_MODIFIER_PUBLIC;
		info.cls->members[ 2 ].offset = offsetof( Camera2DComponent, Camera2DComponent::adjusted );
		info.cls->members[ 2 ].size = sizeof( Camera2DAdjustedOrtho );
		info.cls->members[ 2 ].isStatic = false;
		info.cls->members[ 2 ].isConst = false;
		info.cls->members[ 2 ].isPointer = false;
		info.cls->members[ 2 ].arrayLength = 1;
		info.cls->members[ 2 ].uiSliderMin = v4(0.0);
		info.cls->members[ 2 ].uiSliderMax = v4(0.0);
		info.cls->members[ 2 ].uiSliderSpeed = 0.1;
		info.cls->members[ 2 ].is32BitBool = false;
		info.cls->members[ 2 ].isNoEdit = false;

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < SpriteSubmissionSystem > () { return 29; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<SpriteSubmissionSystem>()
	{
		MetadataInfo info;
		info.id = 29;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "SpriteSubmissionSystem";
		info.cls->baseClassName = "ECSSystem";
		info.cls->baseClassSize = sizeof( ECSSystem );
		info.cls->size = sizeof( SpriteSubmissionSystem );
		info.cls->isComponent = false;
		info.cls->isSystem = true;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< SpriteSubmissionSystem >;
		info.cls->members.SetCapacityAndElementCount( 0 );

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < TransformHierarchy2DSystem > () { return 30; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<TransformHierarchy2DSystem>()
	{
		MetadataInfo info;
		info.id = 30;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "TransformHierarchy2DSystem";
		info.cls->baseClassName = "ECSSystem";
		info.cls->baseClassSize = sizeof( ECSSystem );
		info.cls->size = sizeof( TransformHierarchy2DSystem );
		info.cls->isComponent = false;
		info.cls->isSystem = false;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< TransformHierarchy2DSystem >;
		info.cls->members.SetCapacityAndElementCount( 0 );

		return info;
	}

	template<>
	EU_API metadata_typeid Metadata::GetTypeID < ViewProjection2DSystem > () { return 31; }

	template<>
	MetadataInfo Metadata::ConstructMetadataInfo<ViewProjection2DSystem>()
	{
		MetadataInfo info;
		info.id = 31;
		info.type = METADATA_CLASS;
		info.cls = Eunoia::Metadata::AllocateClass( true );
		info.cls->name = "ViewProjection2DSystem";
		info.cls->baseClassName = "ECSSystem";
		info.cls->baseClassSize = sizeof( ECSSystem );
		info.cls->size = sizeof( ViewProjection2DSystem );
		info.cls->isComponent = false;
		info.cls->isSystem = true;
		info.cls->isEvent = false;
		info.cls->DefaultConstructor = Eunoia::MetadataCreateInstance< ViewProjection2DSystem >;
		info.cls->members.SetCapacityAndElementCount( 0 );

		return info;
	}
	const metadata_typeid Metadata::LastEngineTypeID = 31;

	void Metadata::InitMetadataInfos()
	{
		RegisterMetadataInfo( ConstructMetadataInfo< v2 >() );
		RegisterMetadataInfo( ConstructMetadataInfo< v3 >() );
		RegisterMetadataInfo( ConstructMetadataInfo< v4 >() );
		RegisterMetadataInfo( ConstructMetadataInfo< m3 >() );
		RegisterMetadataInfo( ConstructMetadataInfo< m4 >() );
		RegisterMetadataInfo( ConstructMetadataInfo< quat >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Transform2D >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Transform3D >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Gamepad >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Key >() );
		RegisterMetadataInfo( ConstructMetadataInfo< MouseButton >() );
		RegisterMetadataInfo( ConstructMetadataInfo< GamepadButton >() );
		RegisterMetadataInfo( ConstructMetadataInfo< GamepadTrigger >() );
		RegisterMetadataInfo( ConstructMetadataInfo< GamepadThumbstick >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Transform2DComponent >() );
		RegisterMetadataInfo( ConstructMetadataInfo< SpriteComponent >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Camera2DOrthoType >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Camera2DStandardOrtho >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Camera2DAdjustedOrtho >() );
		RegisterMetadataInfo( ConstructMetadataInfo< Camera2DComponent >() );
		RegisterMetadataInfo( ConstructMetadataInfo< SpriteSubmissionSystem >() );
		RegisterMetadataInfo( ConstructMetadataInfo< TransformHierarchy2DSystem >() );
		RegisterMetadataInfo( ConstructMetadataInfo< ViewProjection2DSystem >() );
	}

}