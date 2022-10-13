nodes = ['', 'W', 'm', '-', 'I', 'P', 'U', '-', 'H', '{', 'U', 'V', 'Q', 'I', 'L', 'A', 'L', 'h', 'G', 'W', 'h', 'i', '5', 'S', '-', '}', 'F', 'c']

paths = [
  (0, 2, 4708),
  (0, 7, 1789),
  (0, 9, 5274),
  (0, 10, 2874),
  (0, 11, 9701),
  (0, 12, 4362),
  (0, 14, 4489),
  (0, 16, 6070),
  (0, 18, 8065),
  (0, 22, 1908),
  (0, 23, 2970),
  (0, 26, 5810),
  (0, 27, 8037),
  (1, 16, 6394),
  (1, 27, 3721),
  (2, 0, 4708),
  (2, 8, 1852),
  (2, 22, 1844),
  (3, 12, 6338),
  (4, 0, 6854),
  (4, 2, 3195),
  (4, 20, 2413),
  (4, 23, 1880),
  (5, 19, 7682),
  (6, 0, 8491),
  (6, 8, 5936),
  (6, 25, 9298),
  (6, 27, 3932),
  (7, 4, 3975),
  (7, 18, 4978),
  (7, 20, 5697),
  (8, 0, 7467),
  (8, 3, 8287),
  (8, 7, 7341),
  (8, 16, 7302),
  (9, 15, 7335),
  (9, 23, 2626),
  (10, 16, 1560),
  (10, 25, 2594),
  (11, 13, 2981),
  (11, 24, 9612),
  (12, 5, 3831),
  (12, 27, 6756),
  (13, 15, 6448),
  (14, 0, 4489),
  (14, 17, 3644),
  (14, 21, 7363),
  (14, 26, 4984),
  (15, 1, 8570),
  (15, 14, 2765),
  (15, 18, 8648),
  (16, 0, 6070),
  (16, 6, 9036),
  (16, 15, 6568),
  (16, 26, 4940),
  (17, 4, 4071),
  (17, 8, 7625),
  (18, 4, 1297),
  (18, 9, 9022),
  (18, 16, 7710),
  (18, 23, 1249),
  (19, 0, 4559),
  (19, 11, 3196),
  (20, 1, 9746),
  (20, 2, 8923),
  (20, 8, 3480),
  (20, 19, 6358),
  (21, 0, 1828),
  (21, 8, 8653),
  (22, 7, 9730),
  (22, 10, 2915),
  (22, 14, 4126),
  (23, 2, 4460),
  (23, 4, 1880),
  (23, 6, 2328),
  (23, 14, 1775),
  (24, 17, 9742),
  (25, 0, 8585),
  (26, 16, 4940),
  (26, 18, 6637),
  (26, 22, 6237),
  (26, 25, 2976),
  (26, 27, 3548),
  (27, 6, 3932),
  (27, 20, 8720),
  (27, 21, 7627)  
]

def Route(FullPath, ActualNode, Distance, UsedNodes):
    if len(FullPath) <= len('FLAG{'):
        if not 'FLAG{'.startswith(FullPath):
            return
        
    if Distance > 163912:
        return

    if Distance == 163912:
        print(FullPath)
        quit()

    for Path in paths:
        if Path[0] == ActualNode:
            if not Path[1] in UsedNodes:
                Route(FullPath + nodes[Path[1]], Path[1], Distance + Path[2], UsedNodes.copy() + [Path[1]])

Route('', 0, 0, [])