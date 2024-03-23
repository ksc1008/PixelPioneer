#version 330 core

#define cplx vec2
#define cplx_new(re, im) vec2(re, im)
#define cplx_re(z) z.x
#define cplx_im(z) z.y
#define cplx_exp(z) (exp(z.x) * cplx_new(cos(z.y), sin(z.y)))
#define cplx_exp_clamp(z) (exp(z.x) * cplx_new(cos(max(min(z.y,PI),-PI)), sin(max(min(z.y,PI/2),-PI/2))))
#define cplx_scale(z, scalar) (z * scalar)
#define cplx_abs(z) (sqrt(z.x * z.x + z.y * z.y))
#define PI 3.1415926538

layout(location = 0) in ivec3 aPos;

out vec2 TexCoord;
out vec3 normal;

out float ao;

flat out int texIdx;

uniform float land_y;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool enableAO;

mat4 inverse(mat4 m) {
	float
		a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
		a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
		a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
		a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

		b00 = a00 * a11 - a01 * a10,
		b01 = a00 * a12 - a02 * a10,
		b02 = a00 * a13 - a03 * a10,
		b03 = a01 * a12 - a02 * a11,
		b04 = a01 * a13 - a03 * a11,
		b05 = a02 * a13 - a03 * a12,
		b06 = a20 * a31 - a21 * a30,
		b07 = a20 * a32 - a22 * a30,
		b08 = a20 * a33 - a23 * a30,
		b09 = a21 * a32 - a22 * a31,
		b10 = a21 * a33 - a23 * a31,
		b11 = a22 * a33 - a23 * a32,

		det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

	return mat4(
		a11 * b11 - a12 * b10 + a13 * b09,
		a02 * b10 - a01 * b11 - a03 * b09,
		a31 * b05 - a32 * b04 + a33 * b03,
		a22 * b04 - a21 * b05 - a23 * b03,
		a12 * b08 - a10 * b11 - a13 * b07,
		a00 * b11 - a02 * b08 + a03 * b07,
		a32 * b02 - a30 * b05 - a33 * b01,
		a20 * b05 - a22 * b02 + a23 * b01,
		a10 * b10 - a11 * b08 + a13 * b06,
		a01 * b08 - a00 * b10 - a03 * b06,
		a30 * b04 - a31 * b02 + a33 * b00,
		a21 * b02 - a20 * b04 - a23 * b00,
		a11 * b07 - a10 * b09 - a12 * b06,
		a00 * b09 - a01 * b07 + a02 * b06,
		a31 * b01 - a30 * b03 - a32 * b00,
		a20 * b03 - a21 * b01 + a22 * b00) / det;
}


vec3 getNormal(){
	float t = (aPos.y&1)*2-1;
	return vec3(((aPos.y & 4) >> 2),
		((7 - (aPos.y & 7)) >> 2),
		((aPos.y & 2) >> 1)) * t;
}

vec3 getPos() {
	return vec3(float(aPos.x % 256), float((aPos.x >> 8) % 256), float(aPos.x >> 16));
}


void main()
{

	mat4 viewModel = inverse(view);
	vec4 pos = model * vec4(getPos(),1);
	vec3 camPos = (viewModel)[3].xyz;
	float rp = 128;

	vec2 planedir = normalize(vec2(pos.x - camPos.x, pos.z - camPos.z));
	cplx plane = cplx_new(pos.y - camPos.y, sqrt((pos.x - camPos.x) * (pos.x - camPos.x) + (pos.z - camPos.z) * (pos.z - camPos.z)));
	cplx circle = rp * cplx_exp_clamp(cplx_scale(plane, 1.0 / rp)) - cplx_new(rp, 0);
	pos.x = cplx_im(circle) * planedir.x + camPos.x;
	pos.z = cplx_im(circle) * planedir.y + camPos.z;
	pos.y = cplx_re(circle) + camPos.y;

	//float pi = -pos.x / r_p;
	//vec4 newPos = vec4((pos.y + r_p) * cos(pi), (pos.y + r_p) * sin(pi),pos.z,1);

	gl_Position = projection * view * pos;
	//gl_Position = projection * view * vec4(aPos.x * 0.5,aPos.y * 0.5,aPos.z * 0.5, 1.0);
	TexCoord = vec2(
		float( (((aPos.y & 8) >> 3) * (aPos.z % 65536))), 
		float( (((aPos.y & 16) >> 4) * (aPos.z >> 16))));
	texIdx = aPos.y >> 6;
	if(enableAO)
		ao = float((aPos.y >> 5) & 1);
	else
		ao = 0;

	normal = getNormal();
}