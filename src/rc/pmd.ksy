meta:
  id: pmd
  file-extension: pmd
  endian: le
seq:
  - id: header
    type: header
  - id: vertex
    type: vertex_with_len
  - id: face_vertex
    type: face_vertex_with_len
  - id: material
    type: material_with_len

types:
  header:
    seq:
      - id: magic
        contents: 'Pmd'
      - id: version
        type: f4
      - id: model_name
        type: strz
        size: 20
        encoding: Shift_JIS
      - id: comment
        type: strz
        size: 256
        encoding: Shift_JIS
  vertex_with_len:
    seq:
      - id: vert_count
        type: u4
      - id: vertex
        type: vertex_t
        repeat: expr
        repeat-expr: vert_count
  vertex_t:
    seq:
      - id: pos
        type: f4
        repeat: expr
        repeat-expr: 3
      - id: normal_vec
        type: f4
        repeat: expr
        repeat-expr: 3
      - id: uv
        type: f4
        repeat: expr
        repeat-expr: 2
      - id: bone_num
        type: u2
        repeat: expr
        repeat-expr: 2
      - id: bone_weight
        type: u1
      - id: edge_flag
        type: u1
  face_vertex_with_len:
    seq:
      - id: face_vert_count
        type: u4
      - id: face_vert_index
        type: u2
        repeat: expr
        repeat-expr: face_vert_count

  material_with_len:
    seq:
      - id: material_count
        type: u4
      - id: material
        type: material_t
        repeat: expr
        repeat-expr: material_count
  material_t:
    seq:
      - id: diffuse_color
        type: f4
        repeat: expr
        repeat-expr: 3
      - id: alpha
        type: f4
      - id: specularity
        type: f4
      - id: specular_color
        type: f4
        repeat: expr
        repeat-expr: 3
      - id: ambient_color
        type: f4
        repeat: expr
        repeat-expr: 3
      - id: toon_index
        type: u1
      - id: edge_flag
        type: u1
      - id: face_vert_count
        type: u4
      - id: texture_file_name
        type: strz
        size: 20
        encoding: Shift_JIS
