int arnold(int argc, char * argv[]){
	int random = rand()%10;
	switch(random){
		case 0:
			printf("Let off some steam, Bennett.\n\n");
			break;
		case 1:
			printf("Sully: Here's twenty dollars to get some beers in Val Verde. It'll give us all a little more time with your daughter.\n");
			printf("Matrix: You're a funny man, Sully, I like you. That's why I'm going to kill you last.\n\n");
			break;
		case 2:
			printf("Remember, Sully, when I promised to kill you last?\n");
			printf("That's right, Matrix. You did.\n");
			printf("I lied.\n\n");
			break;
		case 3:
			printf("Cooke: You scared, motherfucker? Well, you should be, because this Green Beret is going to kick your big ass!\n");
			printf("Matrix: I eat Green Berets for breakfast. And right now, I'm very hungry!\n\n");
			break;
		case 4:
			printf("Matrix: Don't break radio silence until they see me.\n");
			printf("Cindy: How will I know?\n");
			printf("Matrix: Because all fucking hell is going to break loose. \n\n");
			break;
		case 5:
			printf("Gen. Kirby: Keep on the airwaves and let me know if you hear anything unusual.\n");
			printf("Soldier: I'll keep at it. What are you expecting?\n");
			printf("Gen. Kirby: World War Three. \n\n");
			break;
		case 6:
			printf("Cooke: Fuck you, asshole!.\n");
			printf("Matrix: Fuck YOU, asshole!.\n\n");
			break;
		case 7:
			printf("Cindy: What happened to Sully?\n");
			printf("Matrix: I let him go. \n\n");
			break;
		case 8:
			printf("Cindy: You steal my car, you rip the seat out, you kidnap me, you ask me to help you find your daughter which I very kindly do, and then you get me involved in a shoot out where people are dying and there's blood spurting all over the place, and then I watch you rip a phone booth out of a wall, swing from the ceiling like Tarzan, and then there's a cop that's going to shoot you and I save you and they start chasing me. Are you going to tell me what's going on or what?\n");
			printf("Matrix: No. \n\n");
			break;
		case 9:
			printf("Diaz: My people, they got some business with you. And if you want your kid back, then you gotta co-operate, right?\n");
			printf("Matrix: Wrong! \n\n");
			break;
		default:
			printf("RANDOM: %d", random);
			break;
	}
	return 1;
}
