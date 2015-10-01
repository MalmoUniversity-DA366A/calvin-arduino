/*
 * handleMessage.cpp
 *
 * Author: Jesper Hansen
 */

#include "handleMessage.h"

/**
 * Function for handling the which Json message that Calvin-Base is sending.
 * @param &msg JsonObject containing the message from Calvin-Base
 */
int MessageHandler::handleMsg(JsonObject &msg)
{
	/* This won't change, or we would lose the list in memory */
	struct node *root;
	/* This will point to each node as it traverse the list */
	//struct node *conductor;

	// Check if JsonObject msg contains the key: "cmd"
	if (msg.containsKey("cmd"))
	{
		// Get the value from msg key "cmd"
		const char* message = msg["cmd"];

		if (!strcmp(message, "JOIN_REQUEST"))
			{
				root = (node*) malloc(sizeof(nodes));

				if (root == NULL)
				{
					return 9;
				}

				// These method will be activated in the final program
				/*handleJoin(msg);
				root->val = reply;

				handleSetupTunnel(msg);
				root->next = (node*) malloc(sizeof(struct node));
				root->next->val = request;*/

				return 1;
			}

		if (!strcmp(message, "ACTOR_NEW"))
		{
			root = (node*) malloc(sizeof(nodes));
			if (root == NULL)
			{
				return 9;
			}

			// These method will be activated in the final program
			/*root->val = handleActorNew(msg);
			root->next = (node*) malloc(sizeof(struct node));
			root->next->val = handleSetupPorts(msg);*/

			return 2;
		}

		if (!strcmp(message, "TUNNEL_DATA"))
		{
			root = (node*) malloc(sizeof(nodes));
			if (root == NULL)
			{
				return 9;
			}

			// These method will be activad in the final program
			//root->val = handleTunnelData(msg);

			return 3;
		}

		if (!strcmp(message, "TOKEN"))
		{
			root = (node*) malloc(sizeof(nodes));
			if (root == NULL)
			{
				return 9;
			}
			// These method will be activated in the final program
			//root->val = handleToken(msg);

			return 4;
		}

		if (!strcmp(message, "TOKEN_REPLY"))
		{
			// These metod will be activated in the final program
			// Return an empty list with an empty list within it

			return 5;
		}

		if (!strcmp(message, "REPLY")) {
			// These metod will be activated in the final program
			// Return an empty list

			return 6;
		}
		return 7;
	}
	return 8;
}
