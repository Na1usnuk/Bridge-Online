from BridgeGame import BridgeSession


x = BridgeSession()

x.AddPlayer("Nazik")

print(x.ListOfPlayers()[0].nick())
