import sys
import json
import copy
from pprint import pprint

#returns the distance between two molecules by considering their smiles strings
#currently: computes the difference in the number of C's, N's, O's, P's, and S's
def distance(smiles1, smiles2):
    diffC = smiles1.count('C') - smiles2.count('C')
    diffO = smiles1.count('O') - smiles2.count('O')
    diffN = smiles1.count('N') - smiles2.count('N')
    diffS = smiles1.count('S') - smiles2.count('S')
    diffP = smiles1.count('P') - smiles2.count('P')
    dist = (diffC**2+diffO**2+diffN**2+diffS**2+diffP**2)**0.5
    return dist

#removes all the molecules from "data" that are more than a distance of
#"threshold" away from the origin of the tree (ie, node 0)
def removeMolecules(data, threshold):
    smiles1 = data['nodes'][0]['name']
    listOfNodes = copy.deepcopy(data['nodes'])
    removedNodes = []
    for node in listOfNodes:
        if node['group'] == 1:
            smiles2 = node['name']
            if distance(smiles1,smiles2) > float(threshold):
                data['nodes'].remove(node)
                removedNodes.append(node)
    cleanTree(data, removedNodes)

#removes links that point to non-existant nodes
#removes reactions that lead to non-existant molecules (those that have already been removed)
#removes links that lead to reactions removed in the previous step
#renumbers all nodes (and the corresponding link's source/target values) to make them sequential
def cleanTree(data, removedNodes):
    #first, remove links pointing to nonexistant nodes
    listOfLinks = copy.deepcopy(data['links'])
    removedLinks = []
    for link in listOfLinks:
        link_target = link['target']
        link_source = link['source']
        for node in removedNodes:
            if (node['id'] == link_target or node['id'] == link_source):
                removedLinks.append(link)
                data['links'].remove(link)
                break


#    pprint(removedLinks)
    #Now remove all reactions that lead to nonexistant molecules
    listOfNodes = copy.deepcopy(data['nodes'])
    for node in listOfNodes:
        if node['group'] == 2: #is a reaction
            counter = 0
            for link in data['links']:
                if link['target']==node['id'] or link['source']==node['id']:
                    counter = counter+1
            if counter < 2:
                removedNodes.append(node)
                data['nodes'].remove(node)
            
#            remove = False
#            for link in removedLinks:
#                if link['source'] == node['id']:
#                    for remnode in tmpRemovedNodes:
#                        if link['target'] == remnode['id']:
#                            remove = True
#                            break
#                elif link['target'] == node['id']:
#                    for remnode in tmpRemovedNodes:
#                        if link['source'] == remnode['id']:
#                            remove = True
#                            break
#                if remove:
#                    data['nodes'].remove(node)
#                    removedNodes.append(node)
#                    break

    #Now remove all links to reactions that were just removed
    listOfLinks2 = copy.deepcopy(data['links'])
    for link in listOfLinks2:
        link_target = link['target']
        link_source = link['source']
        for node in removedNodes:
            if (node['id'] == link_target or node['id'] == link_source):
                data['links'].remove(link)
                break

    #Now renumber the remaining nodes and the link's source/target values
    renumbering = {} #stores renumbering data: {oldNumber:newNumber}
    counter = 0
    for node in data['nodes']:
        renumbering[node['id']] = counter
        node['id']=counter
        counter = counter + 1
    #now renumber the links
    for link in data['links']:
        link['source'] = renumbering[link['source']]
        link['target'] = renumbering[link['target']]
       

def main(pathtojson, thresh):
    json_data=open(pathtojson+".bak")
    data = json.load(json_data)
    for link in data['links']:
        link['val'] = 100
    
    print "current working SMILES: " + data['nodes'][0]["name"]
    
    mynum = len(data['nodes'])
    
    removeMolecules(data,thresh)
    
    #    print "Number of nodes before filtering: " + str(mynum)
    #    print "Number of nodes after filtering: " + str(len(data['nodes']))
    
    f=open(pathtojson,'w')
    f.write(json.dumps(data,indent=4))
    f.close()
    json_data.close()
    
