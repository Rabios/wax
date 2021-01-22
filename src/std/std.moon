w_trunc = (x) ->
  if x < 0
    return math.ceil(x)
  else
    return math.floor(x)

w_vec_init = (z,n) ->
  x = {}
  for i=1,n do
    x[i]=z
  return x

w_arr_slice = (x,i,n) ->
  y = {}
  for j=(i+1),(i+n) do
    y[j-i]=x[j]
  return y

w_arr_remove = (x,i,n) ->
  for j=(i+n),(i+1),-1 do
    table.remove(x,j)

w_map_len = (x) ->
  cnt = 0
  for k,v in pairs(x) do
    cnt = cnt + 1
  return cnt

w_strcpy = (x,i,n) ->
  return string.sub(x,i+1,i+n)