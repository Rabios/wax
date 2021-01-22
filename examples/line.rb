#########################################
  # line                                #
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

def atan2(x,y) return Math.atan2(x,y) end
def sinh(x)    return Math.sinh(x)    end
def cosh(x)    return Math.cosh(x)    end
def tanh(x)    return Math.tanh(x)    end
def acos(x)    return Math.acos(x)    end
def asin(x)    return Math.asin(x)    end
def atan(x)    return Math.atan(x)    end
def cos(x)     return Math.cos(x)     end
def sin(x)     return Math.sin(x)     end
def tan(x)     return Math.tan(x)    	end
def exp(x)     return Math.exp(x)     end
def log(x)     return Math.log(x)    	end
def sqrt(x)    return Math.sqrt(x)    end
def pow(x,y)   return x.pow(y)        end
def ceil(x)    return x.ceil()        end
def floor(x)   return x.floor()      	end
def fabs(x)    return x.abs()  		    end
def abs(x)     return x.abs()         end
def round(x)   return x.round()       end
def fmin(x,y)  return x < y ? x : y   end
def fmax(x,y)  return x > y ? x : y   end
def random()   return rand()          end
INFINITY = 1 / 0.0

def struct__image()
  this_ = {}
  this_[:data]=nil
  this_[:w]=0
  this_[:h]=0
  return this_
end

def zeros(w,h)
  data=nil
  data=([])
  im=nil
  im=struct__image()
  i=(0)
  while (((i)<(((w)*(h)))) && 1 || 0)!=0
    ((data)[((data).length())] = (0))
    i+=1
  end
  im[:data]=data
  im[:w]=w
  im[:h]=h
  return im
end

def getpixel(im,x,y)
  if (((((((((((((x)<(0)) && 1 || 0))!=0 || ((((y)<(0)) && 1 || 0))!=0) && 1 || 0))!=0 || ((((x)>=(im[:w])) && 1 || 0))!=0) && 1 || 0))!=0 || ((((y)>=(im[:h])) && 1 || 0))!=0) && 1 || 0))!=0
    return 0
  end
  return im[:data][(((((y)*(im[:w])))+(x)))]
end

def setpixel(im,x,y,v)
  if (((((((((((((x)<(0)) && 1 || 0))!=0 || ((((y)<(0)) && 1 || 0))!=0) && 1 || 0))!=0 || ((((x)>=(im[:w])) && 1 || 0))!=0) && 1 || 0))!=0 || ((((y)>=(im[:h])) && 1 || 0))!=0) && 1 || 0))!=0
    return
  end
  im[:data][(((((y)*(im[:w])))+(x)))]=v
end

def linestep(sx0,sx1,sy0,sy1,x)
  if ((((sx0)==(sx1)) && 1 || 0))!=0
    return sy1
  end
  return w_trunc(round(((sy0)+(((((x)-(sx0)))*(((((sy1)-(sy0)))/(((sx1)-(sx0))))))))))
end

def line(im,x0,y0,x1,y1,v)
  sx0=0
  sx0=w_trunc(round(x0))
  sy0=0
  sy0=w_trunc(round(y0))
  sx1=0
  sx1=w_trunc(round(x1))
  sy1=0
  sy1=w_trunc(round(y1))
  dx=0
  dx=(((((sx1)>(sx0)) && 1 || 0))!=0?(1):(-1))
  dy=0
  dy=(((((sy1)>(sy0)) && 1 || 0))!=0?(1):(-1))
  if ((((abs(((sx0)-(sx1))))>=(abs(((sy0)-(sy1))))) && 1 || 0))!=0
    x=0
    x=sx0
    while (((x)!=(((sx1)+(dx)))) && 1 || 0)!=0
      y=0
      y=linestep(sx0,sx1,sy0,sy1,x)
      setpixel(im,x,y,v)
      x=((x)+(dx))
    end
  else
    y=0
    y=sy0
    while (((y)!=(((sy1)+(dy)))) && 1 || 0)!=0
      x=0
      x=linestep(sy0,sy1,sx0,sx1,y)
      setpixel(im,x,y,v)
      y=((y)+(dy))
    end
  end
end

def imshow(im)
  s=nil
  s="+"
  j=(0)
  while (((j)<(im[:w])) && 1 || 0)!=0
    s<<(w_strfix("-"))
    j+=1
  end
  s<<(w_strfix("+\n"))
  i=(0)
  while (((i)<(im[:h])) && 1 || 0)!=0
    s<<(w_strfix("|"))
    j=(0)
    while (((j)<(im[:w])) && 1 || 0)!=0
      s<<(w_strfix(((getpixel(im,j,i))+(32))))
      j+=1
    end
    s<<(w_strfix("|\n"))
    i+=1
  end
  s<<(w_strfix("+"))
  j=(0)
  while (((j)<(im[:w])) && 1 || 0)!=0
    s<<(w_strfix("-"))
    j+=1
  end
  s<<(w_strfix("+"))
  print(s)
  s=nil#(GC)
end

def dispose(im)
  im[:data]=nil#(GC)
  im=nil#(GC)
end

def main()
  im=nil
  im=zeros(60,30)
  i=(0)
  while (((i)<(7)) && 1 || 0)!=0
    x0=0.0
    x0=((random())*(im[:w]))
    y0=0.0
    y0=((random())*(im[:h]))
    x1=0.0
    x1=((random())*(im[:w]))
    y1=0.0
    y1=((random())*(im[:h]))
    v=0
    v=((i)+(1))
    line(im,x0,y0,x1,y1,v)
    i+=1
  end
  imshow(im)
  dispose(im)
  return 0
end

# === User Code            END   ===
exit(main())
