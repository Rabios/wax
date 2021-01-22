def w_trunc(x)
	if (x < 0) then
		return x.ceil()
	else
		return x.floor()
	end
end

def w_arr_remove(x, i, n)
	(i..(i+n)).each do |j|
		x.delete(j)
	end
end

def w_strfix(s)
	if s.class == "Integer"
		return s.chr().downcase()
	end
	return s
end