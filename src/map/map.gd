## Hex map implementation for Godot
## Converted from C++ map.cc and map.hh
## Requires coords.gd

extends RefCounted

const HexCoords = preload("res://coords.gd")

## Map data structures

class Hex:
	var corners: Dictionary = {}  # Dictionary[HexCoords.HexCornerDirection, Corner]
	var edges: Dictionary = {}    # Dictionary[HexCoords.HexEdgeDirection, Edge]
	
	func _init():
		corners = {}
		edges = {}

class Corner:
	var hexes: Dictionary = {}   # Dictionary[HexCoords.HexCornerDirection, Hex]
	var edges: Dictionary = {}   # Dictionary[HexCoords.CornerEdgeDirection, Edge]
	
	func _init():
		hexes = {}
		edges = {}

class Edge:
	var hexes: Dictionary = {}    # Dictionary[HexCoords.HexEdgeDirection, Hex]
	var corners: Dictionary = {}  # Dictionary[HexCoords.CornerEdgeDirection, Corner]
	
	func _init():
		hexes = {}
		corners = {}

## MapBounds class
class MapBounds:
	var min_bounds: HexCoords.HexCoord2
	var max_bounds: HexCoords.HexCoord2
	
	func _init(p_min_bounds: HexCoords.HexCoord2 = null, p_max_bounds: HexCoords.HexCoord2 = null):
		min_bounds = p_min_bounds if p_min_bounds != null else HexCoords.HexCoord2.new()
		max_bounds = p_max_bounds if p_max_bounds != null else HexCoords.HexCoord2.new()
	
	static func from_radius(radius: int) -> MapBounds:
		var bounds: int = radius - 1
		var result = MapBounds.new()
		result.min_bounds = HexCoords.HexCoord2.new(-bounds, -bounds)
		result.max_bounds = HexCoords.HexCoord2.new(bounds, bounds)
		return result
	
	func is_within_bounds(coord: HexCoords.HexCoord2) -> bool:
		return coord.r > min_bounds.r and coord.q > min_bounds.q and \
		       coord.r < max_bounds.r and coord.q < max_bounds.q

## MapCoords iterator class
class MapCoords:
	var size: int
	
	func _init(p_size: int):
		size = p_size
	
	func get_all_coords() -> Array:
		var coords: Array = []
		var r: int = -size
		var max_r: int = size
		
		while r < max_r:
			var q: int = -size + abs(min(r, 0))
			var max_q: int = size - max(0, r)
			
			while q <= max_q:
				coords.append(HexCoords.HexCoord2.new(q, r))
				q += 1
			
			r += 1
		
		return coords

## HexMap class
class HexMap:
	var map_bounds: MapBounds
	var hexes: Dictionary = {}     # Dictionary[HexCoord2, Hex] - keyed by hash
	var corners: Dictionary = {}   # Dictionary[CornerCoord, Corner] - keyed by hash
	var edges: Dictionary = {}     # Dictionary[EdgeCoord, Edge] - keyed by hash
	
	func _init(p_map_bounds: MapBounds):
		map_bounds = p_map_bounds
		hexes = {}
		corners = {}
		edges = {}
	
	func get_normalized_corner_coord(raw_coord: HexCoords.CornerCoord) -> HexCoords.CornerCoord:
		var potential_coord: HexCoords.HexCoord2
		var normalized_coord: HexCoords.HexCoord2 = raw_coord.hex_coord
		var assigned: bool = false
		var normalized_corner_direction: HexCoords.HexCornerDirection = raw_coord.corner_direction
		
		match raw_coord.corner_direction:
			HexCoords.HexCornerDirection.RIGHT, HexCoords.HexCornerDirection.BOTTOM_RIGHT:
				# No normalization needed
				pass
			
			HexCoords.HexCornerDirection.BOTTOM_LEFT:
				# Can be normalized as RIGHT of BOTTOM_LEFT hex
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.BOTTOM_LEFT
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_corner_direction = HexCoords.HexCornerDirection.RIGHT
					assigned = true
			
			HexCoords.HexCornerDirection.LEFT:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP_LEFT
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_corner_direction = HexCoords.HexCornerDirection.BOTTOM_RIGHT
					assigned = true
			
			HexCoords.HexCornerDirection.TOP_LEFT:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP_LEFT
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_corner_direction = HexCoords.HexCornerDirection.RIGHT
					assigned = true
				if not assigned:
					potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
						HexCoords.HexEdgeDirection.TOP
					)
					if map_bounds.is_within_bounds(potential_coord):
						normalized_coord = potential_coord
						normalized_corner_direction = HexCoords.HexCornerDirection.BOTTOM_LEFT
						assigned = true
			
			HexCoords.HexCornerDirection.TOP_RIGHT:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_corner_direction = HexCoords.HexCornerDirection.BOTTOM_RIGHT
					assigned = true
				if not assigned:
					potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
						HexCoords.HexEdgeDirection.TOP
					)
					if map_bounds.is_within_bounds(potential_coord):
						normalized_coord = potential_coord
						normalized_corner_direction = HexCoords.HexCornerDirection.BOTTOM_LEFT
						assigned = true
		
		return HexCoords.CornerCoord.new(normalized_coord, normalized_corner_direction)
	
	func get_normalized_edge_coord(raw_coord: HexCoords.EdgeCoord) -> HexCoords.EdgeCoord:
		var potential_coord: HexCoords.HexCoord2
		var normalized_coord: HexCoords.HexCoord2 = raw_coord.hex_coord
		var normalized_edge_direction: HexCoords.HexEdgeDirection = raw_coord.edge_direction
		
		match raw_coord.edge_direction:
			HexCoords.HexEdgeDirection.BOTTOM_RIGHT, \
			HexCoords.HexEdgeDirection.BOTTOM, \
			HexCoords.HexEdgeDirection.BOTTOM_LEFT:
				# No normalization needed
				pass
			
			HexCoords.HexEdgeDirection.TOP_LEFT:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP_LEFT
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_edge_direction = HexCoords.HexEdgeDirection.BOTTOM_RIGHT
			
			HexCoords.HexEdgeDirection.TOP:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_edge_direction = HexCoords.HexEdgeDirection.BOTTOM
			
			HexCoords.HexEdgeDirection.TOP_RIGHT:
				potential_coord = raw_coord.hex_coord.get_neighbouring_hex_coord(
					HexCoords.HexEdgeDirection.TOP_RIGHT
				)
				if map_bounds.is_within_bounds(potential_coord):
					normalized_coord = potential_coord
					normalized_edge_direction = HexCoords.HexEdgeDirection.BOTTOM_LEFT
		
		return HexCoords.EdgeCoord.new(normalized_coord, normalized_edge_direction)
	
	static func build_map_of_size(map_size: int) -> HexMap:
		var map_bounds := MapBounds.from_radius(map_size)
		var map := HexMap.new(map_bounds)
		var map_coords := MapCoords.new(map_size)
		
		for coord in map_coords.get_all_coords():
			var hex := Hex.new()
			map.hexes[coord.hash()] = hex
			
			# Process corners
			for corner_direction in HexCoords.HEX_CORNER_DIRECTIONS:
				var raw_corner_coord := HexCoords.CornerCoord.new(coord, corner_direction)
				var normalized_corner_coord := map.get_normalized_corner_coord(raw_corner_coord)
				var corner: Corner
				
				var corner_hash := normalized_corner_coord.hash()
				if map.corners.has(corner_hash):
					corner = map.corners[corner_hash]
				else:
					corner = Corner.new()
					map.corners[corner_hash] = corner
				
				hex.corners[corner_direction] = corner
				corner.hexes[HexCoords.opposite_hex_corner_direction(corner_direction)] = hex
			
			# Process edges
			for edge_direction in HexCoords.HEX_EDGE_DIRECTIONS:
				var raw_edge_coord := HexCoords.EdgeCoord.new(coord, edge_direction)
				var normalized_edge_coord := map.get_normalized_edge_coord(raw_edge_coord)
				var edge: Edge
				
				var edge_hash := normalized_edge_coord.hash()
				if map.edges.has(edge_hash):
					edge = map.edges[edge_hash]
				else:
					edge = Edge.new()
					map.edges[edge_hash] = edge
					
					# Map to neighbouring corners
					for edge_to_corner_direction in HexCoords.EDGE_TO_CORNER_DIRECTION_MAPPING[edge_direction]:
						var corner_direction := HexCoords.edge_direction_to_corner_direction(
							edge_direction,
							edge_to_corner_direction
						)
						var corner_coord := HexCoords.CornerCoord.new(coord, corner_direction)
						var normalized_corner_coord := map.get_normalized_corner_coord(corner_coord)
						var corner_hash := normalized_corner_coord.hash()
						var corner_ptr: Corner = map.corners[corner_hash]
						
						corner_ptr.edges[HexCoords.opposite_corner_edge_direction(edge_to_corner_direction)] = edge
						edge.corners[edge_to_corner_direction] = corner_ptr
				
				hex.edges[edge_direction] = edge
				edge.hexes[HexCoords.opposite_hex_edge_direction(edge_direction)] = hex
		
		return map
	
	func get_hexes() -> Dictionary:
		return hexes.duplicate()
	
	func get_hex_at_coord(coord: HexCoords.HexCoord2) -> Hex:
		var hash_val := coord.hash()
		if hexes.has(hash_val):
			return hexes[hash_val]
		return null
