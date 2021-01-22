#########################################
  # nqueens                             #
#########################################
# Compiled by WAXC (Version Jan 10 2021)

# === WAX Standard Library BEGIN ===
def w_trunc(x)
  if (x > 0) then
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
# === WAX Standard Library END   ===

# === User Code            BEGIN ===

def attackable(B,y)
  x=0
  x=B[(y)]
  i=(1)
  while (((i)<=(y)) && 1 || 0)!=0
    t=0
    t=B[(((y)-(i)))]
    if ((((((((((t)==(x)) && 1 || 0))!=0 || ((((t)==(((x)-(i)))) && 1 || 0))!=0) && 1 || 0))!=0 || ((((t)==(((x)+(i)))) && 1 || 0))!=0) && 1 || 0))!=0
      return 1
    end
    i+=1
  end
  return 0
end

def putboard(B,s)
  o=nil
  o="N="
  o<<(w_strfix(((8).to_s())))
  o<<(w_strfix(" solution #"))
  o<<(w_strfix(((s).to_s())))
  o<<(w_strfix("\n"))
  y=(0)
  while (((y)<(8)) && 1 || 0)!=0
    x=(0)
    while (((x)<(8)) && 1 || 0)!=0
      o<<(w_strfix((((((B[(y)])==(x)) && 1 || 0))!=0?("|Q"):("|_"))))
      x+=1
    end
    o<<(w_strfix("|\n"))
    y+=1
  end
  print(o)
  o=nil#(GC)
end

def solve()
  B=nil
  B=Array.new(8,0)
  s=0
  s=0
  y=0
  y=0
  B[(0)]=-1
  while (((y)>=(0)) && 1 || 0)!=0
    B[(y)]=((B[(y)])+(1))
    while ((((((B[(y)])<(8)) && 1 || 0))!=0 && (attackable(B,y))!=0) && 1 || 0)!=0
      B[(y)]=((B[(y)])+(1))
    end
    if ((((B[(y)])<(8)) && 1 || 0))!=0
      if ((((y)<(((8)-(1)))) && 1 || 0))!=0
        y=((y)+(1))
        B[(y)]=-1
      else
        s=((s)+(1))
        putboard(B,s)
      end
    else
      y=((y)-(1))
    end
  end
  B=nil#(GC)
end

def main()
  solve()
  return 0
end

# === User Code            END   ===
exit(main())
