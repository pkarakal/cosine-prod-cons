for producer_count in {1,2,4,8,16,32,64}; do
  for consumer_count in {1,2,4,8,16,32,64}; do
    if [[ $producer_count -lt $consumer_count ]]; then \
      continue
    fi
    for iterations in {10,20,50,100,200,500,1000,1500,2000,2500,5000}; do
      ./cosine_prod_cons -p $producer_count -c $consumer_count -i $iterations -o results/results-"${producer_count}"-"${consumer_count}"-"${iterations}".csv >> /dev/null
    done
  done
done
