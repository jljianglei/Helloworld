#Filename:001.py
cnt = 0#count the sum of result
for i in range(1,5):
	for j in range(1,5):
		for k in range(1,5):
			if i!=j and i!=k and j!=k:
			  print i*100+j*10+k
			  cnt+=1
print cnt
