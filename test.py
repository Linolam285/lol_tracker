import json
import pandas as pd

# Supposons que ton JSON soit dans 'match_data.json'
with open("match.json", "r") as f:
    data = json.load(f)

participants = data['info']['participants']

flattened_data = []

for p in participants:
    flat = {}

    # Champs principaux
    flat['riotIdGameName'] = p.get('riotIdGameName', '')
    flat['championName'] = p.get('championName', '')
    flat['teamId'] = p.get('teamId', 0)

    # Champs à l'intérieur de 'challenges'
    challenges = p.get('challenges', {})
    flat['damagePerMinute'] = challenges.get('damagePerMinute', 0)
    flat['teamDamagePercentage'] = challenges.get('teamDamagePercentage', 0)
    flat['killParticipation'] = challenges.get('killParticipation', 0)

    # Ajoute plus de champs si besoin
    flattened_data.append(flat)

# Convertir en DataFrame pour CSV
df = pd.DataFrame(flattened_data)
df.to_csv("match_flat.csv", index=False)

print("CSV généré avec succès !")