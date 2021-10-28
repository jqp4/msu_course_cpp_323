from pyvis.network import Network
import json

root0 = '/Users/gleb/Projects/C:C++/msu_course_cpp_323/'
root = root0 + '02_knight_and_princess/gleb_skryabin/'
vertexSize = 16
nullVertexColor = '#dd4b39'
defaultVertexColor = '#99ddff'
edgeColors = {'gray': '#595959', 'green': '#5cd65c',
              'blue': '#33ccff', 'yellow': '#e6e600', 'red': '#dd4b39'}

drawOptions = '''{
    "nodes": {
        "color": {
            "border": "rgba(0,0,0,1)",
            "highlight": {
                "border": "rgba(0,0,0,1)",
                "background": "rgba(255,199,143,1)"
            },
            "hover": {
                "border": "rgba(233,96,64,1)"
            }
        },
        "font": {
            "color": "rgba(0,0,0,1)",
            "size": 15,
            "background": "rgba(0,0,0,0)",
            "strokeWidth": 7,
            "strokeColor": "rgba(255,220,136,1)"
        },
        "shapeProperties": {
            "borderRadius": 5
        },
        "size": 30
    },
    "physics": {
        "barnesHut": {
            "gravitationalConstant": -1000,
            "springLength": 10
        },
        "minVelocity": 1.39
    }
}'''


def generateGraph():
    net = Network(height='100%', width='100%', directed=True)
    with open(root + 'graph.json') as json_file:
        data = json.load(json_file)

    for v in data['vertices']:
        c = nullVertexColor if v['id'] == 0 else defaultVertexColor
        net.add_node(v['id'], label=v['depth'], size=vertexSize, color=c)

    for e in data['edges']:
        vs = e['vertex_ids']
        net.add_edge(vs[0], vs[1], color=edgeColors[e['color']])

    neighbor_map = net.get_adj_list()
    for node in net.nodes:
        nid = node['id']
        node['title'] = f'Node {nid} neighbors:<br>' + str(neighbor_map[nid])
        node['neighbor_count'] = len(neighbor_map[nid])

    return net


def main():
    net = generateGraph()
    net.set_options(drawOptions)
    # net.show_buttons(filter_=['physics'])
    net.show(root + 'vis/graph.html')


if __name__ == '__main__':
    main()
