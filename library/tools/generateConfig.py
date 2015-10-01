from Tkinter import *

routingS = ["DirectRouting", "DirectBroadcastRouting", "HierarchicalSpreaditRouting", "ScribeRouting", "SpreaditRouting"]
filterS = ["NullFilter", "BruteForceFilter", "DecisionTreeFilter", "GeneralBooleanExpressionsFilter"]
orderS = ["NullOrder", "GMSOrder", "MTPOrder", "DetMergeOrder"]
deliverS = ["NullDeliver", "AckDeliver", "NackDeliver"]
persistenceS = ["NullPersistence"]
validityS = ["NullValidity", "TimeValidity"]
partitionS = ["NullPartition", "BruteForcePartition", "DirectBroadcastPartition"]
securityS = ["NullSecurity"]
rendezvousS = ["NullRendezvous"]

strategies = [routingS, filterS, orderS, deliverS, persistenceS, validityS, partitionS, securityS, rendezvousS]
stratNames = ["Routing", "Filter", "Order", "Deliver", "Persistence", "Validity", "Partition", "Security", "Rendezvous"]
lbList = []

def end():
	exit(0)

def generate():
	print "TODO: generate header with "
	for l in lbList:
		print "\t", l.get(l.curselection())

root = Tk()

i = 0
for strats in strategies:
	lbl = Label(root, text=stratNames[i])
	lbl.grid(row=0, column=i)
	lb = Listbox(root, exportselection=False, width=15)
	lb.grid(row=1,column=i)
	for strat in strats:
		lb.insert(END, strat)
	lb.selection_set(0)
	lbList.append(lb)
	i += 1

btnGen = Button(root, text="Generate", command=generate)
btnGen.grid(row=2, column=0)

btnEnd = Button(root, text="End", command=end)
btnEnd.grid(row=2, column=1)

root.mainloop()
