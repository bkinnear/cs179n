uniform sampler2D texture;
uniform vec2 center;
uniform float los_radius;
uniform float los_sharpness;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float shadow = 1.0;
    float dist = distance(center, gl_FragCoord.xy);

    if (dist > los_radius) {
        float delta = dist - los_radius;
        shadow = max(0.0, 1.0 - (delta / los_sharpness));
    }

    gl_FragColor = gl_Color * pixel * shadow;
}
