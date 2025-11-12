## Hex map coordinate system for Godot
## Converted from C++ coords.cc and coords.hh

class_name HexCoords
extends RefCounted

## Enum definitions
enum HexEdgeDirection {
	BOTTOM_RIGHT = 0,
	BOTTOM = 1,
	BOTTOM_LEFT = 2,
	TOP_LEFT = 3,
	TOP = 4,
	TOP_RIGHT = 5,
}

enum CornerEdgeDirection {
	RIGHT = 0,
	BOTTOM_RIGHT = 1,
	BOTTOM_LEFT = 2,
	LEFT = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
}

enum HexCornerDirection {
	RIGHT = 0,
	BOTTOM_RIGHT = 1,
	BOTTOM_LEFT = 2,
	LEFT = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
}

## Constants
const HEX_EDGE_DIRECTIONS: Array[HexEdgeDirection] = [
	HexEdgeDirection.BOTTOM_RIGHT,
	HexEdgeDirection.BOTTOM,
	HexEdgeDirection.BOTTOM_LEFT,
	HexEdgeDirection.TOP_LEFT,
	HexEdgeDirection.TOP,
	HexEdgeDirection.TOP_RIGHT,
]

const HEX_CORNER_DIRECTIONS: Array[HexCornerDirection] = [
	HexCornerDirection.RIGHT,
	HexCornerDirection.BOTTOM_RIGHT,
	HexCornerDirection.BOTTOM_LEFT,
	HexCornerDirection.LEFT,
	HexCornerDirection.TOP_LEFT,
	HexCornerDirection.TOP_RIGHT,
]

## Note: Can be optimized. The RIGHT, BOTTOM_LEFT AND TOP_LEFT behave in the
## same way Same as BOTTOM_RIGHT, LEFT and TOP_RIGHT
const CORNER_TO_EDGE_DIRECTION_MAPPING: Array = [
	# HexCornerDirection.RIGHT
	[
		CornerEdgeDirection.RIGHT,
		CornerEdgeDirection.BOTTOM_LEFT,
		CornerEdgeDirection.TOP_LEFT,
	],
	# HexCornerDirection.BOTTOM_RIGHT
	[
		CornerEdgeDirection.BOTTOM_RIGHT,
		CornerEdgeDirection.LEFT,
		CornerEdgeDirection.TOP_RIGHT,
	],
	# HexCornerDirection.BOTTOM_LEFT
	[
		CornerEdgeDirection.RIGHT,
		CornerEdgeDirection.BOTTOM_LEFT,
		CornerEdgeDirection.TOP_LEFT,
	],
	# HexCornerDirection.LEFT
	[
		CornerEdgeDirection.BOTTOM_RIGHT,
		CornerEdgeDirection.LEFT,
		CornerEdgeDirection.TOP_RIGHT,
	],
	# HexCornerDirection.TOP_LEFT
	[
		CornerEdgeDirection.RIGHT,
		CornerEdgeDirection.BOTTOM_LEFT,
		CornerEdgeDirection.TOP_LEFT,
	],
	# HexCornerDirection.TOP_RIGHT
	[
		CornerEdgeDirection.BOTTOM_RIGHT,
		CornerEdgeDirection.LEFT,
		CornerEdgeDirection.TOP_RIGHT,
	],
]

const EDGE_TO_CORNER_DIRECTION_MAPPING: Array = [
	# HexEdgeDirection.BOTTOM_RIGHT
	[
		CornerEdgeDirection.BOTTOM_LEFT,
		CornerEdgeDirection.TOP_RIGHT,
	],
	# HexEdgeDirection.BOTTOM
	[
		CornerEdgeDirection.RIGHT,
		CornerEdgeDirection.LEFT,
	],
	# HexEdgeDirection.BOTTOM_LEFT
	[
		CornerEdgeDirection.BOTTOM_RIGHT,
		CornerEdgeDirection.TOP_LEFT,
	],
	# HexEdgeDirection.TOP_LEFT
	[
		CornerEdgeDirection.BOTTOM_LEFT,
		CornerEdgeDirection.TOP_RIGHT,
	],
	# HexEdgeDirection.TOP
	[
		CornerEdgeDirection.RIGHT,
		CornerEdgeDirection.LEFT,
	],
	# HexEdgeDirection.TOP_RIGHT
	[
		CornerEdgeDirection.BOTTOM_RIGHT,
		CornerEdgeDirection.TOP_LEFT,
	],
]

## Static utility functions
static func opposite_hex_edge_direction(direction: HexEdgeDirection) -> HexEdgeDirection:
	return (direction + 3) % 6 as HexEdgeDirection

static func opposite_hex_corner_direction(direction: HexCornerDirection) -> HexCornerDirection:
	return (direction + 3) % 6 as HexCornerDirection

static func opposite_corner_edge_direction(direction: CornerEdgeDirection) -> CornerEdgeDirection:
	return (direction + 3) % 6 as CornerEdgeDirection

static func edge_direction_to_corner_direction(
	edge_direction: HexEdgeDirection,
	edge_corner_direction: CornerEdgeDirection
) -> HexCornerDirection:
	match edge_direction:
		HexEdgeDirection.BOTTOM_RIGHT:
			match edge_corner_direction:
				CornerEdgeDirection.BOTTOM_LEFT:
					return HexCornerDirection.BOTTOM_RIGHT
				CornerEdgeDirection.TOP_RIGHT:
					return HexCornerDirection.RIGHT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		
		HexEdgeDirection.BOTTOM:
			match edge_corner_direction:
				CornerEdgeDirection.LEFT:
					return HexCornerDirection.BOTTOM_LEFT
				CornerEdgeDirection.RIGHT:
					return HexCornerDirection.BOTTOM_RIGHT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		
		HexEdgeDirection.BOTTOM_LEFT:
			match edge_corner_direction:
				CornerEdgeDirection.BOTTOM_RIGHT:
					return HexCornerDirection.BOTTOM_LEFT
				CornerEdgeDirection.TOP_LEFT:
					return HexCornerDirection.LEFT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		
		HexEdgeDirection.TOP_LEFT:
			match edge_corner_direction:
				CornerEdgeDirection.BOTTOM_LEFT:
					return HexCornerDirection.LEFT
				CornerEdgeDirection.TOP_RIGHT:
					return HexCornerDirection.TOP_LEFT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		
		HexEdgeDirection.TOP:
			match edge_corner_direction:
				CornerEdgeDirection.LEFT:
					return HexCornerDirection.TOP_LEFT
				CornerEdgeDirection.RIGHT:
					return HexCornerDirection.TOP_RIGHT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		
		HexEdgeDirection.TOP_RIGHT:
			match edge_corner_direction:
				CornerEdgeDirection.BOTTOM_RIGHT:
					return HexCornerDirection.RIGHT
				CornerEdgeDirection.TOP_LEFT:
					return HexCornerDirection.TOP_RIGHT
				_:
					push_error("This edge %d does not support this corner %d" % [edge_direction, edge_corner_direction])
					return HexCornerDirection.RIGHT
		_:
			push_error("Unknown edge direction: %d" % edge_direction)
			return HexCornerDirection.RIGHT

## String conversion functions
static func hex_edge_direction_to_string(edge_direction: HexEdgeDirection) -> String:
	match edge_direction:
		HexEdgeDirection.BOTTOM_RIGHT:
			return "EdgeDir<BOTTOM_RIGHT = %d>" % edge_direction
		HexEdgeDirection.BOTTOM:
			return "EdgeDir<BOTTOM = %d>" % edge_direction
		HexEdgeDirection.BOTTOM_LEFT:
			return "EdgeDir<BOTTOM_LEFT = %d>" % edge_direction
		HexEdgeDirection.TOP_LEFT:
			return "EdgeDir<TOP_LEFT = %d>" % edge_direction
		HexEdgeDirection.TOP:
			return "EdgeDir<TOP = %d>" % edge_direction
		HexEdgeDirection.TOP_RIGHT:
			return "EdgeDir<TOP_RIGHT = %d>" % edge_direction
		_:
			return "EdgeDir<UNKNOWN = %d>" % edge_direction

static func hex_corner_direction_to_string(corner_direction: HexCornerDirection) -> String:
	match corner_direction:
		HexCornerDirection.BOTTOM_RIGHT:
			return "CornerDir<BOTTOM_RIGHT = %d>" % corner_direction
		HexCornerDirection.BOTTOM_LEFT:
			return "CornerDir<BOTTOM_LEFT = %d>" % corner_direction
		HexCornerDirection.LEFT:
			return "CornerDir<LEFT = %d>" % corner_direction
		HexCornerDirection.TOP_LEFT:
			return "CornerDir<TOP_LEFT = %d>" % corner_direction
		HexCornerDirection.TOP_RIGHT:
			return "CornerDir<TOP_RIGHT = %d>" % corner_direction
		HexCornerDirection.RIGHT:
			return "CornerDir<RIGHT = %d>" % corner_direction
		_:
			return "CornerDir<UNKNOWN = %d>" % corner_direction

static func corner_edge_direction_to_string(corner_edge_direction: CornerEdgeDirection) -> String:
	match corner_edge_direction:
		CornerEdgeDirection.RIGHT:
			return "CornerEdgeDirection.RIGHT"
		CornerEdgeDirection.BOTTOM_RIGHT:
			return "CornerEdgeDirection.BOTTOM_RIGHT"
		CornerEdgeDirection.BOTTOM_LEFT:
			return "CornerEdgeDirection.BOTTOM_LEFT"
		CornerEdgeDirection.LEFT:
			return "CornerEdgeDirection.LEFT"
		CornerEdgeDirection.TOP_LEFT:
			return "CornerEdgeDirection.TOP_LEFT"
		CornerEdgeDirection.TOP_RIGHT:
			return "CornerEdgeDirection.TOP_RIGHT"
		_:
			return "CornerEdgeDirection.UNKNOWN"


## HexCoord2 class
class HexCoord2:
	var q: int
	var r: int
	
	func _init(p_q: int = 0, p_r: int = 0):
		q = p_q
		r = p_r
	
	func equals(other: HexCoord2) -> bool:
		return q == other.q and r == other.r
	
	func add(other: HexCoord2) -> HexCoord2:
		return HexCoord2.new(q + other.q, r + other.r)
	
	func get_neighbouring_hex_coord(neighbour_direction: HexEdgeDirection) -> HexCoord2:
		match neighbour_direction:
			HexEdgeDirection.TOP:
				return HexCoord2.new(q, r - 1)
			HexEdgeDirection.TOP_RIGHT:
				return HexCoord2.new(q + 1, r - 1)
			HexEdgeDirection.BOTTOM_RIGHT:
				return HexCoord2.new(q + 1, r)
			HexEdgeDirection.BOTTOM:
				return HexCoord2.new(q, r + 1)
			HexEdgeDirection.BOTTOM_LEFT:
				return HexCoord2.new(q - 1, r + 1)
			HexEdgeDirection.TOP_LEFT:
				return HexCoord2.new(q - 1, r)
			_:
				return HexCoord2.new(q, r)
	
	func get_edge_coord(edge_direction: HexEdgeDirection) -> EdgeCoord:
		return EdgeCoord.new(self, edge_direction)
	
	func get_corner_coord(corner_direction: HexCornerDirection) -> CornerCoord:
		return CornerCoord.new(self, corner_direction)
	
	func to_string() -> String:
		return "HexCoord2<q=%d, r=%d>" % [q, r]
	
	func _to_string() -> String:
		return to_string()
	
	func hash() -> int:
		# Simple hash combining q and r
		var result := 0
		result = result ^ (hash(q) + 0x9e3779b9 + (result << 6) + (result >> 2))
		result = result ^ (hash(r) + 0x9e3779b9 + (result << 6) + (result >> 2))
		return result


## EdgeCoord class
class EdgeCoord:
	var hex_coord: HexCoord2
	var edge_direction: HexEdgeDirection
	
	func _init(p_hex_coord: HexCoord2 = null, p_edge_direction: HexEdgeDirection = HexEdgeDirection.BOTTOM_RIGHT):
		hex_coord = p_hex_coord if p_hex_coord != null else HexCoord2.new()
		edge_direction = p_edge_direction
	
	func equals(other: EdgeCoord) -> bool:
		return edge_direction == other.edge_direction and hex_coord.equals(other.hex_coord)
	
	func to_string() -> String:
		return "EdgeCoord<hex_coord=%s, edge_direction=%s>" % [
			hex_coord.to_string(),
			HexCoords.hex_edge_direction_to_string(edge_direction)
		]
	
	func _to_string() -> String:
		return to_string()
	
	func hash() -> int:
		var result := 0
		result = result ^ (hex_coord.hash() + 0x9e3779b9 + (result << 6) + (result >> 2))
		result = result ^ (hash(edge_direction) + 0x9e3779b9 + (result << 6) + (result >> 2))
		return result


## CornerCoord class
class CornerCoord:
	var hex_coord: HexCoord2
	var corner_direction: HexCornerDirection
	
	func _init(p_hex_coord: HexCoord2 = null, p_corner_direction: HexCornerDirection = HexCornerDirection.RIGHT):
		hex_coord = p_hex_coord if p_hex_coord != null else HexCoord2.new()
		corner_direction = p_corner_direction
	
	func equals(other: CornerCoord) -> bool:
		return corner_direction == other.corner_direction and hex_coord.equals(other.hex_coord)
	
	func to_string() -> String:
		return "CornerCoord<hex_coord=%s, corner_direction=%s>" % [
			hex_coord.to_string(),
			HexCoords.hex_corner_direction_to_string(corner_direction)
		]
	
	func _to_string() -> String:
		return to_string()
	
	func hash() -> int:
		var result := 0
		result = result ^ (hex_coord.hash() + 0x9e3779b9 + (result << 6) + (result >> 2))
		result = result ^ (hash(corner_direction) + 0x9e3779b9 + (result << 6) + (result >> 2))
		return result
