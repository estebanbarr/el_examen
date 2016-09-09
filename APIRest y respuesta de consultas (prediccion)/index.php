<?php
include_once('Slim/Slim.php');
include_once('constantes.php');

function getClima($dia)
{
    global $db_user, $db_pass, $db_host, $db_name;
    $db = new mysqli($db_host, $db_user, $db_pass, $db_name);
    if ($db->connect_errno)
        return false;

    $sql  = " select hay_sequia, hay_lluvia, hay_condic_optima ".
            "   from condicion_metereologica ".
            "  where dia = $dia ";

    $res = $db->query($sql);
    if ($res == false)
        return "no hay pronostico para el dia numero [$dia]";

    $r = $res->fetch_object();
    if ($r->hay_sequia == 1)
        return "sequia";
    else if ($r->hay_lluvia == 1)
        return "lluvia";
    else if ($r->hay_condic_optima == 1)
        return "condicion optima de presion y temperatura";
    else
        return "solo puedo decir que no hay lluvia";
}

\Slim\Slim::registerAutoloader();
$app = new \Slim\Slim();

$app->get('/clima',
    function() use ($app)
    {
        $dia = $app->request()->get('dia');
        if ($dia == null)
        {
            $app->halt(400);
            return false;
        }

        $out = array();
        $out['dia'] = $dia;

        $res = getClima($dia);
        if ($res == false) //Error en la conexion de DB...
        {
            $app->halt(500);
            return false;
        }

        $out['clima'] = $res;

        echo json_encode($out);
        return true;
    }
    );

$app->run();
?>