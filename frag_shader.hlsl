uniform sampler2D texture;
uniform vec2 center;
uniform float sqr_los_radius;
uniform float sqr_los_sharpness;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float shadow = 1.0;
    vec2 distanceVec = gl_FragCoord.xy - center;
    float sqr_dist = dot(distanceVec, distanceVec);

    if (sqr_dist > sqr_los_radius) {
        float delta = sqr_dist - sqr_los_radius;
        shadow = max(0.0, 1.0 - (delta/sqr_los_sharpness));
    }

    gl_FragColor = gl_Color * pixel * shadow;
}
