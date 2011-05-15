-- Add missing Ancient Orc Ancestor to database (by Bastek)
DELETE from creature where id=18688;
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) VALUES
(18688, 530, 1, 1, 0, 0, -2300.29, 8571.08, -22.0957, 3.4185, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2407.95, 8655.04, -19.7505, 0.0357841, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2530.94, 8629.31, -25.5884, 3.86067, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2571.84, 8558.23, -33.6043, 4.57538, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2218.21, 8513.48, -17.1368, 0.723004, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2114.42, 8743.48, 11.699, 0.526655, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2017.31, 8743.44, 19.5169, 0.491312, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1998.29, 8843.03, 20.4006, 0.979956, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1922.55, 8890.31, 37.9049, 5.5981, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1812.61, 8813.18, 30.2393, 4.74993, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1750.87, 8877.25, 31.67, 5.45286, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1673.49, 8771.72, 31.4229, 5.01696, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1539.61, 8686.33, 24.643, 4.96591, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1637.01, 8569.54, -12.3352, 4.45932, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1653.31, 8494.49, -12.8156, 0.353261, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1579.32, 8426.67, -9.33804, 4.07055, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1040.87, 7895.8, 22.8202, 2.68398, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1059.54, 7750.35, 25.8758, 3.82595, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -908.965, 7783.44, 40.1036, 2.92902, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -886.413, 7824.75, 40.0754, 2.64864, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -878.863, 7881.25, 38.6467, 0.768393, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -1866.27, 8098.74, -20.3459, 2.59506, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2106.82, 8191.13, -15.5604, 2.34766, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2084.75, 8286.22, -11.4935, 2.8298, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2206.14, 8306.9, -26.191, 1.74673, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2256, 8478.68, -24.6155, 4.09507, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2336.12, 8477.34, -30.3842, 2.60674, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2350.53, 8641.59, -15.5674, 4.43672, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2379.14, 8531.65, -28.7321, 4.69983, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2411.61, 8492.45, -33.4541, 5.29045, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2416.02, 8330.76, -37.6002, 0.420193, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2336.04, 8288.74, -36.3567, 4.24665, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2336.11, 8225.27, -35.7699, 5.05954, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2258.45, 8221.08, -31.2193, 3.52288, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2203.79, 8149.55, -25.1002, 5.98511, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2184.8, 8029.08, -17.6373, 2.84822, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2297.12, 7848.94, -30.4255, 0.900436, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2544.55, 7818.57, -45.0012, 1.78794, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2555.36, 7930.22, -55.675, 2.91341, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2622.32, 7987.03, -48.9066, 0.305103, 300, 0, 0, 6542, 0, 0, 0),
(18688, 530, 1, 1, 0, 0, -2126.06, 8148.39, -15.8518, 2.97938, 300, 0, 0, 6542, 0, 0, 0);