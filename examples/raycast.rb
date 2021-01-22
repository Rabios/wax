#########################################
  # raycast                             #
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
def fmin(x,y)  return x < y ? x : y 	end
def fmax(x,y)  return x > y ? x : y  	end
def random()   return rand()          end
INFINITY = 1 / 0.0

def struct__ray()
  this_ = {}
  this_[:o]=nil
  this_[:d]=nil
  this_[:tmin]=0.0
  this_[:tmax]=0.0
  return this_
end

def struct__mesh()
  this_ = {}
  this_[:vertices]=nil
  this_[:faces]=nil
  this_[:facenorms]=nil
  return this_
end

def v_sub(u,v)
  return ([(((u[(0)])-(v[(0)]))),(((u[(1)])-(v[(1)]))),(((u[(2)])-(v[(2)])))])
end

def v_cross(u,v)
  return ([(((((u[(1)])*(v[(2)])))-(((u[(2)])*(v[(1)]))))),(((((u[(2)])*(v[(0)])))-(((u[(0)])*(v[(2)]))))),(((((u[(0)])*(v[(1)])))-(((u[(1)])*(v[(0)])))))])
end

def v_dot(u,v)
  return ((((((u[(0)])*(v[(0)])))+(((u[(1)])*(v[(1)])))))+(((u[(2)])*(v[(2)]))))
end

def v_scale(u,x)
  return ([(((u[(0)])*(x))),(((u[(1)])*(x))),(((u[(2)])*(x)))])
end

def v_mag(v)
  return sqrt(((((((v[(0)])*(v[(0)])))+(((v[(1)])*(v[(1)])))))+(((v[(2)])*(v[(2)])))))
end

def normalize(v)
  l=0.0
  l=v_mag(v)
  v[(0)]=((v[(0)])/(l))
  v[(1)]=((v[(1)])/(l))
  v[(2)]=((v[(2)])/(l))
end

def det(a,b,c)
  d=nil
  d=v_cross(a,b)
  e=0.0
  e=v_dot(d,c)
  d=nil#(GC)
  return e
end

def new_ray(ox,oy,oz,dx,dy,dz)
  r=nil
  r=struct__ray()
  o=nil
  o=([(ox),(oy),(oz)])
  d=nil
  d=([(dx),(dy),(dz)])
  normalize(d)
  r[:o]=o
  r[:d]=d
  r[:tmin]=0.0
  r[:tmax]=INFINITY
  return r
end

def destroy_ray(r)
  r[:o]=nil#(GC)
  r[:d]=nil#(GC)
  r=nil#(GC)
end

def ray_tri(r,p0,p1,p2)
  e1=nil
  e1=v_sub(p1,p0)
  e2=nil
  e2=v_sub(p2,p0)
  s=nil
  s=v_sub(r[:o],p0)
  _d=nil
  _d=v_scale(r[:d],-1)
  denom=0.0
  denom=det(e1,e2,_d)
  if ((((denom)==(0)) && 1 || 0))!=0
    e1=nil#(GC)
    e2=nil#(GC)
    s=nil#(GC)
    _d=nil#(GC)
    return INFINITY
  end
  u=0.0
  u=((det(s,e2,_d))/(denom))
  v=0.0
  v=((det(e1,s,_d))/(denom))
  t=0.0
  t=((det(e1,e2,s))/(denom))
  if ((((((((((((((((u)<(0)) && 1 || 0))!=0 || ((((v)<(0)) && 1 || 0))!=0) && 1 || 0))!=0 || ((((((1)-(((u)+(v)))))<(0)) && 1 || 0))!=0) && 1 || 0))!=0 || ((((t)<(r[:tmin])) && 1 || 0))!=0) && 1 || 0))!=0 || ((((t)>(r[:tmax])) && 1 || 0))!=0) && 1 || 0))!=0
    e1=nil#(GC)
    e2=nil#(GC)
    s=nil#(GC)
    _d=nil#(GC)
    return INFINITY
  end
  r[:tmax]=t
  e1=nil#(GC)
  e2=nil#(GC)
  s=nil#(GC)
  _d=nil#(GC)
  return t
end

def ray_mesh(r,m,l)
  dstmin=0.0
  dstmin=INFINITY
  argmin=0
  argmin=-1
  i=(0)
  while (((i)<(((m[:faces]).length()))) && 1 || 0)!=0
    a=nil
    a=m[:vertices][(m[:faces][(i)][(0)])]
    b=nil
    b=m[:vertices][(m[:faces][(i)][(1)])]
    c=nil
    c=m[:vertices][(m[:faces][(i)][(2)])]
    t=0.0
    t=ray_tri(r,a,b,c)
    if ((((t)<(dstmin)) && 1 || 0))!=0
      dstmin=t
      argmin=i
    end
    i+=1
  end
  if (((((((argmin)<(-1)) && 1 || 0))!=0 || ((((dstmin)==(INFINITY)) && 1 || 0))!=0) && 1 || 0))!=0
    return 0.0
  end
  n=nil
  n=m[:facenorms][(argmin)]
  ndotl=0.0
  ndotl=v_dot(n,l)
  return ((fmax(ndotl,0))+(0.1))
end

def add_vert(m,x,y,z)
  ((m[:vertices])[((m[:vertices]).length())] = (([(x),(y),(z)])))
end

def add_face(m,a,b,c)
  ((m[:faces])[((m[:faces]).length())] = (([(((a)-(1))),(((c)-(1))),(((b)-(1)))])))
end

def calc_facenorms(m)
  i=(0)
  while (((i)<(((m[:faces]).length()))) && 1 || 0)!=0
    a=nil
    a=m[:vertices][(m[:faces][(i)][(0)])]
    b=nil
    b=m[:vertices][(m[:faces][(i)][(1)])]
    c=nil
    c=m[:vertices][(m[:faces][(i)][(2)])]
    e1=nil
    e1=v_sub(a,b)
    e2=nil
    e2=v_sub(b,c)
    n=nil
    n=v_cross(e1,e2)
    normalize(n)
    ((m[:facenorms])[((m[:facenorms]).length())] = (n))
    e1=nil#(GC)
    e2=nil#(GC)
    i+=1
  end
end

def move_mesh(m,x,y,z)
  i=(0)
  while (((i)<(((m[:vertices]).length()))) && 1 || 0)!=0
    m[:vertices][(i)][(0)]=((m[:vertices][(i)][(0)])+(x))
    m[:vertices][(i)][(1)]=((m[:vertices][(i)][(1)])+(y))
    m[:vertices][(i)][(2)]=((m[:vertices][(i)][(2)])+(z))
    i+=1
  end
end

def destroy_mesh(m)
  i=(0)
  while (((i)<(((m[:vertices]).length()))) && 1 || 0)!=0
    m[:vertices][(i)]=nil#(GC)
    i+=1
  end
  m[:vertices]=nil#(GC)
  i=(0)
  while (((i)<(((m[:faces]).length()))) && 1 || 0)!=0
    m[:faces][(i)]=nil#(GC)
    i+=1
  end
  m[:faces]=nil#(GC)
  i=(0)
  while (((i)<(((m[:facenorms]).length()))) && 1 || 0)!=0
    m[:facenorms][(i)]=nil#(GC)
    i+=1
  end
  m[:facenorms]=nil#(GC)
  m=nil#(GC)
end

def render(m,light)
  pix=nil
  pix=Array.new(3840,0.0)
  normalize(light)
  palette=nil
  palette="`.-,_:^!~;r+|()=>l?icv[]tzj7*f{}sYTJ1unyIFowe2h3Za4X%5P$mGAUbpK960#H&DRQ80WMB@N"
  lo=0.0
  lo=INFINITY
  hi=0.0
  hi=0
  y=(0)
  while (((y)<(40)) && 1 || 0)!=0
    x=(0)
    while (((x)<(80)) && 1 || 0)!=0
      fx=0.0
      fx=((((x)-(((80)/(2.0)))))/(2.0))
      fy=0.0
      fy=((y)-(((40)/(2.0))))
      r=nil
      r=new_ray(0,0,0,fx,fy,100)
      gray=0.0
      gray=ray_mesh(r,m,light)
      hi=fmax(gray,hi)
      if ((((gray)>(0)) && 1 || 0))!=0
        lo=fmin(gray,lo)
      end
      pix[(((((y)*(80)))+(x)))]=gray
      destroy_ray(r)
      x+=1
    end
    y+=1
  end
  s=nil
  s=""
  y=(0)
  while (((y)<(40)) && 1 || 0)!=0
    x=(0)
    while (((x)<(80)) && 1 || 0)!=0
      gray=0.0
      gray=pix[(((((y)*(80)))+(x)))]
      if ((((gray)!=(0)) && 1 || 0))!=0
        gray=((((gray)-(lo)))/(((hi)-(lo))))
        ch=0
        ch=((palette).bytes()[w_trunc(((gray)*(78)))])
        s<<(w_strfix(ch))
      else
        s<<(w_strfix((' ').bytes()[0]))
      end
      x+=1
    end
    s<<(w_strfix("\n"))
    y+=1
  end
  print(s)
  pix=nil#(GC)
  s=nil#(GC)
end

def dodecahedron()
  m=nil
  m=struct__mesh()
  m[:vertices]=([])
  m[:faces]=([])
  m[:facenorms]=([])
  add_vert(m,-0.436466,-0.668835,0.601794)
  add_vert(m,0.918378,0.351401,-0.181931)
  add_vert(m,0.886304,-0.351401,-0.301632)
  add_vert(m,-0.886304,0.351401,0.301632)
  add_vert(m,-0.918378,-0.351401,0.181931)
  add_vert(m,0.132934,0.858018,0.496117)
  add_vert(m,-0.048964,0.981941,-0.182738)
  add_vert(m,0.106555,0.162217,-0.980985)
  add_vert(m,-0.582772,0.162217,-0.796280)
  add_vert(m,-0.132934,-0.858018,-0.496117)
  add_vert(m,0.048964,-0.981941,0.182738)
  add_vert(m,0.582772,-0.162217,0.796280)
  add_vert(m,-0.106555,-0.162217,0.980985)
  add_vert(m,0.436466,0.668835,-0.601794)
  add_vert(m,0.730785,0.468323,0.496615)
  add_vert(m,-0.678888,0.668835,-0.302936)
  add_vert(m,-0.384570,0.468323,0.795474)
  add_vert(m,0.384570,-0.468323,-0.795474)
  add_vert(m,0.678888,-0.668835,0.302936)
  add_vert(m,-0.730785,-0.468323,-0.496615)
  add_face(m,19,3,2)
  add_face(m,12,19,2)
  add_face(m,15,12,2)
  add_face(m,8,14,2)
  add_face(m,18,8,2)
  add_face(m,3,18,2)
  add_face(m,20,5,4)
  add_face(m,9,20,4)
  add_face(m,16,9,4)
  add_face(m,13,17,4)
  add_face(m,1,13,4)
  add_face(m,5,1,4)
  add_face(m,7,16,4)
  add_face(m,6,7,4)
  add_face(m,17,6,4)
  add_face(m,6,15,2)
  add_face(m,7,6,2)
  add_face(m,14,7,2)
  add_face(m,10,18,3)
  add_face(m,11,10,3)
  add_face(m,19,11,3)
  add_face(m,11,1,5)
  add_face(m,10,11,5)
  add_face(m,20,10,5)
  add_face(m,20,9,8)
  add_face(m,10,20,8)
  add_face(m,18,10,8)
  add_face(m,9,16,7)
  add_face(m,8,9,7)
  add_face(m,14,8,7)
  add_face(m,12,15,6)
  add_face(m,13,12,6)
  add_face(m,17,13,6)
  add_face(m,13,1,11)
  add_face(m,12,13,11)
  add_face(m,19,12,11)
  calc_facenorms(m)
  return m
end

def main()
  m=nil
  m=dodecahedron()
  move_mesh(m,0,0,5)
  light=nil
  light=([(0.1),(0.2),(0.4)])
  render(m,light)
  destroy_mesh(m)
  light=nil#(GC)
  return 0
end

# === User Code            END   ===
exit(main())
