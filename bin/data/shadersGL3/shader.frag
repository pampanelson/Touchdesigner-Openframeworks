#version 150

// this is how we receive the texture
uniform sampler2D backbuffer;
uniform sampler2D tex0;

in vec2 texCoordVarying;

out vec4 outputColor;

// float diffU = 0.25f;
// float diffV = 0.04f;
// float f = 0.0195f;
// float k = 0.066f;

// float kernel[9];
// vec2 offset[9];

void main()
{
  vec2 st = texCoordVarying;

  // kernel[0] = 0.707106781;
  // kernel[1] = 1.0;
  // kernel[2] = 0.707106781;
  // kernel[3] = 1.0;
  // kernel[4] = -6.82842712;
  // kernel[5] = 1.0;
  // kernel[6] = 0.707106781;
  // kernel[7] = 1.0;
  // kernel[8] = 0.707106781;

  // offset[0] = vec2( -1.0, -1.0) / vec2(1280.0, 720.0);
  // offset[1] = vec2(  0.0, -1.0) / vec2(1280.0, 720.0);
  // offset[2] = vec2(  1.0, -1.0) / vec2(1280.0, 720.0);
  // offset[3] = vec2( -1.0,  0.0) / vec2(1280.0, 720.0);
  // offset[4] = vec2(  0.0,  0.0) / vec2(1280.0, 720.0);
  // offset[5] = vec2(  1.0,  0.0) / vec2(1280.0, 720.0);
  // offset[6] = vec2( -1.0,  1.0) / vec2(1280.0, 720.0);
  // offset[7] = vec2(  0.0,  1.0) / vec2(1280.0, 720.0);
  // offset[8] = vec2(  1.0,  1.0) / vec2(1280.0, 720.0);

  // vec2 texColor = texture2D(backbuffer, st).rb;
  // float srcTexColor = texture2D(tex0, st).r;

  // vec2 laplace = vec2(0.0);
  // for (int i=0; i < 9; i++)
  // {
  //   laplace += texture2D(tex0, st + offset[i]).rb * kernel[i];
  // }

  // float F = f + srcTexColor * 0.025 - 0.0005;
  // float K = k + srcTexColor * 0.025 - 0.0005;

  // float u = texColor.r;
  // float v = texColor.g + srcTexColor * 0.5;

  // float uvv = u * exp2(v);

  // float du = diffU * laplace.r - uvv + F * (1.0 - u);
  // float dv = diffV * laplace.g + uvv - (F - K) * v;

  // u += du * 0.6;
  // v += dv * 0.6;

  //outputColor = vec4(clamp(u, 0.0, 1.0), 1.0 - u/v, clamp(v, 0.0, 1.0), 1.0);
  outputColor = vec4(texture2D(backbuffer, st).rgb, 1.0);
  // outputColor = vec4(texture2D(backbuffer, st).rgb, 1.0);
}