#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 e = (targetPos - actorPos).Normalize();
	double v_mag = actor->gMaxSpeed;
	Vdesired = v_mag * e;
	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 e = (targetPos - actorPos).Normalize();
	double v_mag = actor->gMaxSpeed;
	Vdesired = v_mag * -e;
	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 e = targetPos - actorPos;
	Vdesired = actor->KArrival * e;
	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 e = targetPos - actorPos;
	Vdesired = actor->KDeparture * (-e) * 2 / (e.Length() * e.Length());
	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	//vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 targetPos = m_pTarget->getGlobalTranslation();
	vec3 e = targetPos - m_actorPos;
	Varrival = actor->KArrival * e;
	//Varrival = m_actorVel;

	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 



	// Step 2. compute Lb
	//TODO: add your code here

	double Lb = actor->TAvoid * m_actorVel.Length();

	// Step 3. find closest obstacle 
	//TODO: add your code here

	vec3 p = m_actorPos/* + m_actorVel.Normalize() * Lb*/;
	double d_flag = 99999999.9;
	Obstacle temp_o;
	for (int i = 0; i < mObstacles->size(); i++)
	{
		std::cout << i << std::endl;
		vec3 p_obstacle = mObstacles->at(i).m_Center.getLocalTranslation();
		vec3 d_world = p_obstacle - p;
		
		double dx = m_actorVel / m_actorVel.Length() * d_world;
		if ((dx < d_flag) && (dx > 0))
		{
			d_flag = dx;
			m_obstaclePos = p_obstacle;
			temp_o = mObstacles->at(i);
		}
	}


	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	bool col = false;	
	vec3 d = /*temp_o.m_Center.getGlobalTranslation()*/m_obstaclePos - p;
	double dy = sqrt(d.Length() * d.Length() - d_flag * d_flag);

	double rb = actor->gAgentRadius;

	double r0 = temp_o.m_Radius;
	if (d_flag <= (Lb + rb + r0))
	{
		if (dy <= (rb + r0))
		{
			col = true;
		}
	}

	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here
	vec3 dy_vector = d - m_actorVel / m_actorVel.Length()*d_flag;
	if (col)
	{
		double avoid_mag = 0.0;
		if (dy <= (rb + r0))
		{
		   avoid_mag = actor->KAvoid*((rb + r0) - dy) / (rb + r0);
		}
		vec3 avoid_direct = -dy_vector / dy_vector.Length();
		Vavoid = avoid_direct * avoid_mag;
	}

	Vdesired = Varrival + Vavoid;
	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TODO: add your code here
	vec3 v0 = vec3(1, 0, 0);
	vec3 noise = vec3(rand(), 0, rand());
	noise = noise / noise.Length();

	// Step2. scale it with a noise factor
	//TODO: add your code here
	noise = actor->KNoise * noise;

	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here
	m_Wander =  actor->KWander * (m_Wander + noise) / (m_Wander + noise).Length();

	std::cout << actor->KWander << std::endl;
	//std::cout << "vec3(" << m_Wander[0] << ", " << m_Wander[1] << ", " << m_Wander[2] << ") " << std::endl;

	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here
	Vdesired = m_Wander + v0;

	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here
	
	if (actor == leader)
	{
		vec3 e = targetPos - actorPos;
		Vdesired = actor->KArrival * e;
	}
	else 
	{
		double radius = actor->gKNeighborhood;
		double allWeights = 0.00;
		vec3 allVector = vec3(0.0);
		for (int i = 0; i < agentList.size(); i++)
		{
			
			BehaviorController* temp = agentList.at(i).getBehaviorController();
			vec3 v = temp->getPosition();
			double distance = (actorPos - v).Length();
			if ((distance < radius) && (distance > 0.00001))
			{
				if (temp->getVelocity().Length() > 0)
				{
					allVector = allVector + distance / radius * temp->getVelocity();
					allWeights = allWeights + distance / radius;
				}
			}
		}

		Vdesired = actor->KAlignment * allVector / allWeights;
		if (allWeights == 0.0)
		{
			vec3 e = targetPos - actorPos;
			Vdesired = actor->KArrival * e;
		}
	}


	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	double radius = actor->gKNeighborhood;
	vec3 dis_vector = vec3(0.0);
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController *temp = agentList.at(i).getBehaviorController();
		double distance = (temp->getPosition() - actorPos).Length();		
		if ((distance < radius) && (distance > 0.00001))
		{
			vec3 direct = actorPos - temp->getPosition();
			dis_vector = dis_vector + (distance / radius) *(direct / (direct.Length() * direct.Length()));

		}
	}

	Vdesired = actor->KSeparation * dis_vector;



	if (Vdesired.Length() < 5.0)
		Vdesired = vec3(0.0);
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 
	vec3 allPos = vec3(0.0);
	double allMass = 0.00;
	double allWeights = 0.00;
	double radius = actor->gKNeighborhood;
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController* temp = agentList.at(i).getBehaviorController();
		double distance = (temp->getPosition() - actorPos).Length();
		if ((distance > 0.00001) && (distance < radius))
		{
			allMass = temp->gMass + allMass;
			allPos = allPos + temp->getPosition() * temp->gMass;
		}
	}
	vec3 center = allPos / allMass;
	Vdesired = center - actorPos;

	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	double radius = actor->gKNeighborhood;
	vec3 targetPos = m_pTarget->getLocalTranslation();
	// compute Vdesired = Vflocking
	// TODO: add your code here 

	// ========================= seperate ============================//
	vec3 VSeparate = vec3(0.0);
	vec3 dis_vector = vec3(0.0);
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController *sep_temp = agentList.at(i).getBehaviorController();
		double sep_distance = (sep_temp->getPosition() - actorPos).Length();
		if ((sep_distance < radius) && (sep_distance > 0.00001))
		{
			vec3 direct = actorPos - sep_temp->getPosition();
			dis_vector = dis_vector + (sep_distance / radius) *(direct / (direct.Length() * direct.Length()));

		}
	}
	VSeparate = actor->KSeparation * dis_vector;
	if (VSeparate.Length() < 5.0)
		VSeparate = vec3(0.0);
	// ========================= Alignment ============================//

	vec3 VAlignment = vec3(0.0);
	BehaviorController* leader = agentList[0].getBehaviorController(); 

	if (actor == leader)
	{
		vec3 e = targetPos - actorPos;
		VAlignment = actor->KArrival * e;
	}
	else
	{
		double align_allWeights = 0.00;
		vec3 align_allVector = vec3(0.0);
		for (int i = 0; i < agentList.size(); i++)
		{

			BehaviorController* align_temp = agentList.at(i).getBehaviorController();
			vec3 v = align_temp->getPosition();
			double distance = (actorPos - v).Length();
			if ((distance < radius) && (distance > 0.00001))
			{
				if (align_temp->getVelocity().Length() > 0)
				{
					align_allVector = align_allVector + distance / radius * align_temp->getVelocity();
					align_allWeights = align_allWeights + distance / radius;
				}
			}
		}

		VAlignment = actor->KAlignment * align_allVector / align_allWeights;
		if (align_allWeights == 0.0)
		{
			vec3 e = targetPos - actorPos;
			VAlignment = actor->KArrival * e;
		}
	}
	// ========================= Coheshion ============================//
	vec3 VCoheshion = vec3(0.0);
	vec3 cohe_allPos = vec3(0.0);
	double allMass = 0.00;
	double allWeights = 0.00;
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController* cohe_temp = agentList.at(i).getBehaviorController();
		double distance = (cohe_temp->getPosition() - actorPos).Length();
		if ((distance > 0.00001) && (distance < radius))
		{
			allMass = cohe_temp->gMass + allMass;
			cohe_allPos = cohe_allPos + cohe_temp->getPosition() * cohe_temp->gMass;
		}
	}
	vec3 center = cohe_allPos / allMass;
	VCoheshion = center - actorPos;

	double c1 = 0.3;
	double c2 = 0.5;
	double c3 = 0.1;

	Vdesired = c1 * VSeparate + c2 * VAlignment + c3 * VCoheshion;
	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;
	vec3 VSep = vec3(0.0);
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent
	vec3 e = targetPos - actorPos; 
	vec3 VArri = actor->KArrival * e;
	if (actor == leader)
	{
		Vdesired = VArri;
	}
	else
	{
		double radius = actor->gKNeighborhood;
		vec3 dis_vector = vec3(0.0);
		for (int i = 0; i < agentList.size(); i++)
		{
			BehaviorController *temp = agentList.at(i).getBehaviorController();
			double distance = (temp->getPosition() - actorPos).Length();
			if ((distance < radius) && (distance > 0.00001))
			{
				vec3 direct = actorPos - temp->getPosition();
				dis_vector = dis_vector + (distance / radius) *(direct / (direct.Length() * direct.Length()));

			}
		}

		VSep = actor->KSeparation * dis_vector;
		if (VSep.Length() < 5.0)
			VSep = vec3(0.0);
		double ca = 0.9;
		double cs = 1;

		Vdesired = ca * VArri + cs * VSep;
	}






	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

