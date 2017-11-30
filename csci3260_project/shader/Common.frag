#version 430

in vec2 UV;

out vec4 daColor;

uniform sampler2D myTextureSampler;


// for fog effect
const vec3 fogColor = vec3(0.5, 0.5,0.5);
const float FogDensity = 0.05f;
in vec4 viewSpace;


//for lighting
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 ambientLight;
uniform vec3 coefficient_d; // diffuse
uniform vec3 coefficient_s; // specular
uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;
uniform vec4 lightColor;



void main()
{
	vec3 Material_Clr = texture( myTextureSampler, UV).rgb;

	//daColor = vec4(Material_Clr, 1.0);



//--------------------------------------------------------------------------------------------------------------------------------------------------------
	//material color
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = ambientLight * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.5,0.5,0.5);
	//diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld * vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld)) ;
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0) ;

	//specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	vec4 specularLight = vec4(s, 0, 0, 1);

	
	//			material_ambient_color							+	material_diffuse_color
	//daColor = ambientLight * texture(myTextureSampler, UV).rgb  + texture(myTextureSampler, UV).rgb* brightness_d * clamp(diffuseLight, 0, 1) *  + specularLight * brightness_s  ;

	daColor = Material_Clr + MaterialAmbientColor  + coefficient_d * clamp(diffuseLight, 0, 1) +  coefficient_s * specularLight    ;

//--------------------------------------------------------------------------------------------------------------------------------------------------------


	// for fog effect
	/*vec3 finalColor = vec3(0.0, 0.0, 0.0);
	float dist =  length(viewSpace);

	float fogFactor = 0;	//visibility
	fogFactor = 1.0 /exp(dist * FogDensity);
    fogFactor = clamp(fogFactor, 0.0, 1.0 );
 
    // mix function fogColor?(1?fogFactor) + lightColor?fogFactor
    finalColor = mix(fogColor, daColor, fogFactor);
	daColor = vec4(finalColor, 1.0);*/



}