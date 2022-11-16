uniform vec2 resolution;
uniform int sceltaFS;
uniform float iTime;
in vec4 ourColor;
const float BEAT = 4.0;

float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float cells(vec2 uv){  // Trimmed down.
    uv = mix(sin(uv + vec2(1.57, 0)), sin(uv.yx*1.4 + vec2(1.57, 0)), .75);
    return uv.x*uv.y*.3 + .7;
}
float fbm(vec2 uv)
{
    
    float f = 200.0;
    vec2 r = (vec2(.9, .45));    
    vec2 tmp;
    float T = 100.0 + iTime * 1.3;
    T += sin(iTime * BEAT) * .1;
    // layers of cells with some scaling and rotation applied.
    for (int i = 1; i < 8; ++i)
    {
        float fi = float(i);
        uv.y -= T * .5;
        uv.x -= T * .4;
        tmp = uv;
        
        uv.x = tmp.x * r.x - tmp.y * r.y; 
        uv.y = tmp.x * r.y + tmp.y * r.x; 
        float m = cells(uv);
        f = smin(f, m, .07);
    }
    return 1. - f;
}
vec3 ld = normalize(vec3(1.0, 2.0, 3.));
vec3 g(vec2 uv)
{
    vec2 off = vec2(0.0, .03);
    float t = fbm(uv);
    float x = t - fbm(uv + off.yx);
    float y = t - fbm(uv + off);
    float s = .0025;
    vec3 xv = vec3(s, x, 0);
    vec3 yv = vec3(0, y, s);
    return normalize(cross(xv, -yv)).xzy;
}
vec2 rand2(vec2 p)
{
	vec2 q = vec2(dot(p, vec2(120.0, 300.0)), dot(p, vec2(270.0, 401.0)));
	return fract(sin(q) * 46111.1111);
}

float rand(vec2 p)
{
	return fract(sin(dot(p, vec2(445.5, 360.535))) * 812787.111);
}

vec2 voronoi(in vec2 pos, float distortion)
{
	vec2 cell = floor(pos);
	vec2 cellOffset = fract(pos);
    float borderDist = 8.0;
    float color;
    
	// sample nearby cells
	for (int x=-1; x<=1; x++)
    {
        for (int y=-1; y<=1; y++)
        {
            vec2 samplePos = vec2(float(y), float(x));
            vec2 center = rand2(cell + samplePos) * distortion;
            vec2 r = samplePos - cellOffset + center;
            float d = dot(r, r);
            float col = rand(cell + samplePos);

            if (d < borderDist)
            {
                borderDist = d;
                color = col;
            }
        }
    }
    // calc border distance
    borderDist = 8.0;
    for (int j=-1; j<=1; j++)
    {
        for (int i=-1; i<=1; i++)
        {
            vec2 samplePos = vec2(float(i),float(j));
            vec2 center = rand2(cell + samplePos) * distortion;
            vec2 r = samplePos + center - cellOffset;

            if (dot(r, r) > 0.000001)
            {
                borderDist = min(borderDist, dot(0.5 * r, normalize(r)));
            }
        }
    }
    return vec2(color, borderDist);
}

void main(){
  if(sceltaFS == 0){
	gl_FragColor = vec4(ourColor);
  }
  else if(sceltaFS ==1){
	vec2 uv = gl_FragCoord.xy / resolution.xx;
	vec2 noise = voronoi(8.0 * uv, 1.0);
    gl_FragColor = vec4(noise.y, noise.y, noise.x, 1.0);
  }
  else{
	vec2 uv = gl_FragCoord.xy / (resolution.xy/0.999);
    uv -= vec2(0.5);  
    float a = (resolution.x)/ (resolution.y/0.999);
    uv.y /= a;
    vec2 ouv = uv;
    float B = sin(iTime * BEAT);
    uv = mix(uv, uv * sin(B), .035);
    vec2 _uv = uv * 25.;
    float f = fbm(_uv);
    
    // base color
    gl_FragColor = vec4(f);
    gl_FragColor.rgb *= vec3(1., .3 + B * .05, 0.1 + B * .05);
    
    vec3 v = normalize(vec3(uv, 1.));
    vec3 grad = g(_uv);
    
    // spec
    vec3 H = normalize(ld + v);
    float S = max(0., dot(grad, H));
    S = pow(S, 4.0) * .2;
    gl_FragColor.rgb += S * vec3(.4, .7, .7);
    // rim
    float R = 1.0 - clamp(dot(grad, v), .0, 1.);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, vec3(.8, .8, 1.), smoothstep(-.2, 2.9, R));
    // edges
    gl_FragColor.rgb = mix(gl_FragColor.rgb, vec3(0.), smoothstep(.45, .55, (max(abs(ouv.y * a), abs(ouv.x)))));
    
    // contrast
    gl_FragColor = smoothstep(.0, 1., gl_FragColor);
}
}