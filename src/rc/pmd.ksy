meta:
  id: pmd
  file-extension: pmd
  endian: le
seq:
  - id: header
    type: header
# - id: logical_screen
#   type: logical_screen
types:
  header:
    seq:
      - id: magic
        contents: 'PMD'
      - id: version
        type: f4
      - id: model_name
        type: str
        size: 20
        encoding: ASCII
      - id: comment
        type: str
        size: 256
        encoding: ASCII
#  logical_screen:
#    seq:
#      - id: image_width
#        type: u2
#      - id: image_height
#        type: u2
#      - id: flags
#        type: u1
#      - id: bg_color_index
#        type: u1
#      - id: pixel_aspect_ratio
#        type: u1
#
#
#        // ヘッダ
#        byte[3] magic;
#        <floatj version;
#        byte[20] modelName;
#        byte[256] comment;
#
#        // 頂点リスト
#        <int vertCount;
#        vertex [vertCount]{
#            <floatj[3] pos;
#            <floatj[3] normalVec;
#            <floatj[2] uv;
#            <short[2] boneNum;
#            byte boneWeight;
#            byte edgeFlag;
#        }
#
#        // 面頂点リスト
#        <int faceVertCount;
#        <short[faceVertCount] faceVertIndex;
#
#        // 材質リスト
#        <int materialCount;
#        material [materialCount]{
#            <floatj[3] diffuseColor;
#            <floatj alpha;
#            <floatj specularity;
#            <floatj[3] specularColor;
#            <floatj[3] ambientColor;
#            byte toonIndex;
#            byte edgeFlag;
#            <int faceVertCount;
#            byte[20] textureFileName;
#        }
