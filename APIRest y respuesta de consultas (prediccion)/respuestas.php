<?php

include_once('constantes.php');

//Calculo los dias...
$dias_anios = array('v'=>72,
                    'f'=>360,
                    'b'=>120);
$tipo = $_POST['tipo'];
$dias = 10 * $dias_anios[$tipo];

//Conexion a DB...
$db = new mysqli($db_host, $db_user, $db_pass, $db_name);

//Consultas...
function getCantPeriodos($condicion)
{
    global $db;

    $sql  = " select count(*) as cantidad ".
            "   from condicion_metereologica ".
            "  $condicion ";

    $res = $db->query($sql);
    if ($res == false)
        return 0;

    $r = $res->fetch_object();
    return $r->cantidad;
}

function getCantPeriodosDeSequia($dias)
{
    return getCantPeriodos(" where hay_sequia = 1 and dia <= $dias ");
}

function getCantPeriodosDeLluvia($dias)
{
    return getCantPeriodos(" where hay_lluvia = 1 and dia <= $dias ");
}

function getDiasPicoMaxDeLluvia($dias)
{
    global $db;

    $sql  = " select perimetro, dia ".
            "   from condicion_metereologica ".
            "  where hay_lluvia = 1 ".
            "    and dia <= $dias ".
            "  order by perimetro desc, dia asc ";

    $dias = array();

    $res = $db->query($sql);
    if ($res == false)
        return $dias;

    $max_perimetro = -2;
    while ($r = $res->fetch_assoc())
    {
        if ($max_perimetro == -2)
            $max_perimetro = $r['perimetro'];
        if ($max_perimetro != $r['perimetro'])
            break;
        $dias[] = $r['dia'];
    }

    return $dias;
}

function getCantPeriodosCondicOptimas($dias)
{
    return getCantPeriodos(" where hay_condic_optima = 1 and dia <= $dias ");
}

?>
<html>
    <head>
        <title>Star trek info clima</title>
    </head>
    <body>
        <p>Calculando en <?php echo $dias; ?> d&iacute;as...</p>
        <table>
            <tr>
                <td>Per&iacute;odos de sequ&iacute;a: </td>
                <td><?php echo getCantPeriodosDeSequia($dias); ?></td>
            </tr>
            <tr>
                <td>Per&iacute;odos de condiciones &oacute;ptimas de presi&oacute;n y temperatura: </td>
                <td><?php echo getCantPeriodosCondicOptimas($dias); ?></td>
            </tr>
            <tr>
                <td>Per&iacute;odos de lluvia: </td>
                <td><?php echo getCantPeriodosDeLluvia($dias); ?></td>
            </tr>
            <tr>
                <td>D&iacute;as de pico m&aacute;ximo de lluvia: </td>
                <td><?php
                    $pmdl = getDiasPicoMaxDeLluvia($dias);
                    echo count($pmdl).": ";
                ?>
                    <select>
                <?php
                        foreach ($pmdl as $d)
                        {
                ?>
                        <option><?php echo $d; ?></option>
                <?php
                        }
                ?>
                    </select>
                </td>
            </tr>
            <tr>
                <form id="f" action="info.php" method="post">
                <td><input type="submit" value="Volver"/></td>
                </form>
            </tr>
        </table>
    </body>
</html>