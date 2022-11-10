

import sys
import subprocess
import sys

def install(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])


try:
    import graphviz
except:
    install('graphviz')
    import graphviz
try:
    import pandas as pd
except:
    install('pandas')
    import pandas as pd
    
dot = graphviz.Digraph('process', format = 'png')

data = pd.read_table(sys.argv[2],names=["id","pid","time"],delimiter=",")

dot.node(str(int(data.iloc[0].id)))

for i in range(1,len(data)):
    if(sys.argv[1] == "-y"):
        max_time = data[data["pid"] == data.iloc[i].pid].time.max()
        if(data.iloc[i].time == max_time):
            dot.node(str(int(data.iloc[i].id)),color = "blue")
    elif(sys.argv[1] == "-e"):
        min_time = data[data["pid"] == data.iloc[i].pid].time.min()
        if(data.iloc[i].time == min_time):
            dot.node(str(int(data.iloc[i].id)),color = "red")
    
    dot.edge(str(int(data.iloc[i].pid)),str(int(data.iloc[i].id)))

dot.render()
